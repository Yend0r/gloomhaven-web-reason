open ScenarioApi;

type fetchStatus = 
    | Loaded
    | Loading
    | FetchError;

type componentState = {
    status: fetchStatus,
    character: option(CharacterApi.character),
    scenario: option(ScenarioApi.scenario),
    fetchError: option(string),
    submissionError: option(string)
};

type componentAction =
    | UpdateScenario(ScenarioApi.scenario)
    | Loaded(CharacterApi.character, ScenarioApi.scenario)
    | SetFetchError(string)
    | SetSubmissionError(string);

let component = ReasonReact.reducerComponent("ScenarioDetails");

let make = (~characterId: int, _children) => {
    ...component,

    initialState: () => {
        status: Loading, 
        character: None, 
        scenario: None, 
        fetchError: None,
        submissionError: None
    },
 
    reducer: (action, state) => {
        switch action {
            | UpdateScenario(scenario) => {
                ReasonReact.Update({...state, scenario: Some(scenario)});
            }
            | Loaded(character, scenario) => 
                ReasonReact.Update(
                    {
                        character: Some(character), 
                        scenario: Some(scenario), 
                        status: Loaded, 
                        fetchError: None, 
                        submissionError: None
                    })
            | SetFetchError(errorMsg) => 
                ReasonReact.Update(
                    {
                        character: None, 
                        scenario: None, 
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
        let onFetchError = (error) => self.send(SetFetchError(error));

        let onFetchSuccess = (character: CharacterApi.character, scenario: ScenarioApi.scenario) => 
            self.send(Loaded(character, scenario));

        let onFetchCharacterSuccess = (character: CharacterApi.character) => 
            ScenarioApi.get(character.id, onFetchSuccess(character), onFetchError); 
        
        CharacterApi.get(characterId, onFetchCharacterSuccess, onFetchError);
    },

    render: ({state, send}) => { 

        let onCancel = () => Routes.navigate(Routes.Character(List));

        let onError = (error) => send(SetSubmissionError(error));
        let onSuccess = (scenario) => send(UpdateScenario(scenario));

        let onHpUpdate = (hp) => 
            ScenarioApi.hpUpdate(characterId, hp, onSuccess, onError); 
        
        let onXpUpdate = (xp) => 
            ScenarioApi.xpUpdate(characterId, xp, onSuccess, onError); 
        
        let onDrawCard = () => 
            ScenarioApi.drawCard(characterId, onSuccess, onError); 
        
        let onReshuffle = () => 
            ScenarioApi.reshuffle(characterId, onSuccess, onError);
            
        
        let onComplete = () => {
            let onError = (error) => send(SetSubmissionError(error));
            let onCompleteSuccess = () => Routes.navigate(Routes.Character(List));
            ScenarioApi.completeScenario(characterId, onCompleteSuccess, onError);
        };

        let displayData = (optCharacter, optScenario) => {
            switch (optCharacter, optScenario) {
            | (Some(character), Some(scenario)) => {  
                <ScenarioDetailsView
                    character
                    scenario
                    onHpUpdate 
                    onXpUpdate
                    onDrawCard
                    onReshuffle
                    onComplete
                    onCancel  />    
            }            
            | _ => 
                <p>(Elem.string("Could not find scenario."))</p>
            };
        };

        switch (state.status) {
            | Loading => 
                <div>(Elem.string("loading..."))</div>
            | FetchError => 
                <GenericError error=state.fetchError />
            | Loaded => 
                {displayData(state.character, state.scenario)}
        };
    }
};