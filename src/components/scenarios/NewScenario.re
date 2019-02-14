
type state = {
    submissionError: option(string)
};

type action =
    | SetSubmissionError(string);

let component = ReasonReact.reducerComponent("NewScenario");

let make = (~characterId, _children) => {
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
 
        let onCancel = () => Routes.navigate(Routes.Character(List));

        let onSubmit = (name) => {
            let onError = (msg) => send(SetSubmissionError(msg));
            let onSuccess = () => Routes.navigate(Routes.Character(Scenario(characterId)));

            ScenarioApi.newScenario(characterId, name, onSuccess, onError); 
        };

        <div className="columns">
            <div className="column is-4">
                <div className="content is-medium">
                    <h3 className="title is-3">(Elem.string("New Scenario"))</h3>
                    <NewScenarioForm 
                        onSubmit={onSubmit} 
                        onCancel={onCancel} 
                        submissionError=?{state.submissionError} />
                </div>
            </div>
        </div>
    }
};