
type state = {
    submissionError: option(string)
};

type action =
    | SetSubmissionError(string);

let component = ReasonReact.reducerComponent("CharacterAdd");

let make = (_children) => {
    ...component,

    initialState: () => { 
        submissionError: None 
    },

    reducer: (action, _state) => {
        switch action {
        | SetSubmissionError(errorMsg) => 
            ReasonReact.Update({submissionError: Some(errorMsg)})
        };
    },

    render: (self) => {   
 
        let showList = () => Routes.navigate(Routes.Character(List));

        let onSubmit = (newCharacter) => {
            let onError = (_error) => 
                (); /*Api.processApiError(error, msg => send(SetSubmissionError(msg)));*/
            let onSuccess = () => 
                showList(); 

            CharacterApi.add(newCharacter, onSuccess, onError); 
        };

        <div className="columns">
            <div className="column is-4">
                <div className="content is-medium">
                    <h3 className="title is-3">(Elem.string("New Character"))</h3>
                    <CharacterAddForm 
                        onSubmit={onSubmit} 
                        onCancel={showList} 
                        submissionError=?{self.state.submissionError} />
                </div>
            </div>
        </div>
    }
};