
type componentState = {
    name: string,
    fieldErrors: list(FormUtils.fieldError)
};

type componentAction =
    | SetFieldErrors(list(FormUtils.fieldError))
    | ClearErrors
    | ChangeName(string);

let component = ReasonReact.reducerComponent("NewScenarioForm");

let make = (~onSubmit: string => unit, ~onCancel, ~submissionError=?, _children) => {
    ...component,

    initialState: () => { 
        name: "",   
        fieldErrors: [] 
    },

    reducer: (action, state) => {
        switch action {
        | SetFieldErrors(errors) => ReasonReact.Update({...state, fieldErrors: errors})
        | ClearErrors => ReasonReact.Update({...state, fieldErrors: []})
        | ChangeName(text) => ReasonReact.Update({...state, name: text})
        };
    },

    render: ({state, send}) => {   

        let onChangeName = (event) => send(ChangeName(event |> Utils.valueFromEvent));
        
        let submit = () => onSubmit(state.name);

        let validate: unit => unit = () => {
            send(ClearErrors);
            
            let fieldErrors = 
                []
                |> FormUtils.validateRequiredString(state.name, "name", "Character name is required.")

            switch(fieldErrors) {
            | [] => submit()
            | _ => send(SetFieldErrors(fieldErrors))
            };              
        };

        let cancel = () => onCancel(); 

        let fieldHasError = (fname) => {
            let error = FormUtils.tryFindError(fname, state.fieldErrors);
            switch(error){
            | Some(_) => true
            | None => false
            };
        };

        let getInputCss = (fname, fieldCss) => 
            switch(fieldHasError(fname)){
            | true => fieldCss ++ " is-danger"
            | false => fieldCss
            };

        <form>
            <FormError validationError={None} submissionError />

            <div className="field">
                <label className="label">(Elem.string("Name"))</label>
                <div className="control">
                    <input 
                        className=(getInputCss("name", "input"))
                        type_="text"
                        id="name"
                        placeholder="Name"
                        name="name"
                        value=state.name
                        onChange=onChangeName
                    />
                </div>
            </div>

            <div className="field is-grouped">
                <div className="control">
                    <UIButton 
                        title="Submit" 
                        onClick=validate 
                        classes="button is-primary" />
                </div>
                <div className="control">
                    <UIButton 
                        title="Cancel" 
                        onClick=cancel 
                        classes="button is-light" />
                </div>
            </div>
        </form>
    }
};