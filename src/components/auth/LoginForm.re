type loginFormState = {
    email: string,
    password: string,
    error: option(string)
};

type action =
    | SetError(string)
    | ClearError
    | ChangeEmail(string)
    | ChangePassword(string);

let component = ReasonReact.reducerComponent("LoginForm");

let make = (~onSubmit, ~submissionError=?, _children) => {
    ...component,

    initialState: () => {
        email: "", 
        password: "", 
        error: None
    },

    reducer: (action, state) => {
        switch action {
        | SetError(errorMsg) => ReasonReact.Update({...state, error: Some(errorMsg)})
        | ClearError => ReasonReact.Update({...state, error: None})
        | ChangeEmail(text) => ReasonReact.Update({...state, email: text})
        | ChangePassword(text) => ReasonReact.Update({...state, password: text})
        }
    },

    render: ({state, send}) => {   

        let changeEmail = (event) => send(ChangeEmail(event |> Utils.valueFromEvent));
        let changePassword = (event) => send(ChangePassword(event |> Utils.valueFromEvent));

        let submit = () => {
            send(ClearError);
          
            let emailValid = Utils.stringIsEmail(state.email);
            let pwdValid = !Utils.stringIsEmpty(state.password);

            switch (emailValid, pwdValid) {
            | (false, false) => send(SetError("Please enter a valid email and password."));
            | (false, true) => send(SetError("Please enter a valid email."));
            | (true, false) => send(SetError("Please enter a password."));
            | (true, true) => onSubmit(state.email, state.password);
            };
        };

        <div className="container has-text-centered">
            <div className="column is-4 is-offset-4">
                <h3 className="title has-text-grey">
                    (Elem.string("GloomHaven"))
                    <div>(Elem.string("Character Manager"))</div>
                </h3>
                <p className="subtitle has-text-grey">(Elem.string("Please login to proceed."))</p>
                <div className="box">
                    <form>
                        <FormError validationError={state.error} submissionError />
                        <div className="field">
                            <div className="control">
                                <input 
                                    type_="email" 
                                    className="input is-large"
                                    placeholder="Your Email"
                                    id="userEmail"
                                    name="userEmail"
                                    value=state.email
                                    onChange=(changeEmail)
                                />
                            </div>
                        </div>

                        <div className="field">
                            <div className="control">
                                <input 
                                    type_="password" 
                                    className="input is-large"
                                    placeholder="Your Password"
                                    id="userPassword"
                                    name="userPassword"
                                    value=state.password
                                    onChange=(changePassword)
                                />
                            </div>
                        </div>
                        <button 
                            className="button is-block is-primary is-large is-fullwidth"
                            onClick=(Utils.handleBtnClick(submit))>
                            (Elem.string("Login"))
                        </button>
                    </form>
                </div>
                <p className="has-text-grey">
                    <a href="../">(Elem.string("Forgot Password (not working yet)"))</a> 
                </p>
            </div>
        </div>
    }
}; 
