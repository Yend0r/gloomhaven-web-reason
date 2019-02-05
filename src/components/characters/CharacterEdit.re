type fetchStatus = 
    | Loaded
    | Loading
    | FetchError;

type componentState = {
    status: fetchStatus,
    character: option(CharacterApi.character),
    ghClass: option(GameDataApi.ghClass),
    fetchError: option(string),
    submissionError: option(string)
};

type componentAction =
    | Loaded(CharacterApi.character, GameDataApi.ghClass)
    | SetFetchError(string)
    | SetSubmissionError(string);

let component = ReasonReact.reducerComponent("CharacterEdit");

let make = (~characterId: int, _children) => {
    ...component,

    initialState: () => {
        status: Loading, 
        character: None, 
        ghClass: None, 
        fetchError: None,
        submissionError: None
    },
 
    reducer: (action, state) => {
        switch action {
            | Loaded(character, ghClass) => 
                ReasonReact.Update(
                    {
                        character: Some(character), 
                        ghClass: Some(ghClass), 
                        status: Loaded, 
                        fetchError: None, 
                        submissionError: None
                    })
            | SetFetchError(errorMsg) => 
                ReasonReact.Update(
                    {
                        character: None, 
                        ghClass: None, 
                        status: FetchError, 
                        fetchError: Some(errorMsg), 
                        submissionError: None
                    })
            | SetSubmissionError(errorMsg) => 
                ReasonReact.Update(
                    {
                        ...state, 
                        fetchError: None, 
                        submissionError: Some(errorMsg)
                    })
        }
    },

    didMount: self => {
        let onFetchError = (error) => 
            ApiUtils.processApiError(error, msg => self.send(SetFetchError(msg)));

        let onFetchSuccess = (character: CharacterApi.character, ghClass: GameDataApi.ghClass) => 
            self.send(Loaded(character, ghClass));

        let onFetchCharacterSuccess = (character: CharacterApi.character) => 
            GameDataApi.get(character.ghClassName, onFetchSuccess(character), onFetchError); 
        
        CharacterApi.get(characterId, onFetchCharacterSuccess, onFetchError);
    },

    render: ({state, send}) => { 

        let gotoList = () => Routes.navigate(Routes.Character(List));

        let onSubmit = (characterUpdate) => {
            let onError = (error) => ApiUtils.processApiError(error, msg => send(SetSubmissionError(msg)));
            let onSuccess = () => gotoList(); 
  
            CharacterApi.update(characterId, characterUpdate, onSuccess, onError); 
        };

        let displayData = (optCharacter, optGhClass) => {
            switch (optCharacter, optGhClass) {
            | (Some(character), Some(ghClass)) => 
                <div className="columns">
                    <div className="column is-6">
                        <h3 className="title is-3">(Elem.string("Edit Character"))</h3>
                        <CharacterEditForm 
                            character
                            ghClass
                            onSubmit={onSubmit} 
                            onCancel={gotoList} 
                            submissionError=?{state.submissionError} />
                    </div>
                </div>
                
            | _ => 
                <p>(Elem.string("Could not find character."))</p>
            };
        };

        switch (state.status) {
            | Loading => 
                <div>(Elem.string("loading..."))</div>
            | FetchError => 
                <GenericError error=state.fetchError />
            | Loaded => 
                {displayData(state.character, state.ghClass)}
        };
    }
};