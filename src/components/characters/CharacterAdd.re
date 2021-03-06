
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

    render: ({state, send}) => {   
 
        let showList = () => Routes.navigate(Routes.Character(List));

        let onSubmit = (newCharacter) => {
            let onError = (msg) => send(SetSubmissionError(msg));
            let onSuccess = () => showList(); 

            CharacterApi.add(newCharacter, onSuccess, onError); 
        };

        <div className="columns">
            <div className="column is-4">
                <div className="content is-medium">
                    <h3 className="title is-3">(Elem.string("New Character"))</h3>
                    <CharacterAddForm 
                        onSubmit={onSubmit} 
                        onCancel={showList} 
                        submissionError=?{state.submissionError} />
                </div>
            </div>
        </div>
    }
};