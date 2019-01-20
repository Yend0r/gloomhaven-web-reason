[@bs.module] external logo : string = "../../images/logo.svg";

type componentState = {
    error: option(string)
};

type action =
    | SetError(string);

let component = ReasonReact.reducerComponent("Login");

let make = (_children) => {

    ...component,

    initialState: () => {
        error: None
    },

    reducer: (action, _state) => {
        switch action {
            | SetError(errorMsg) => ReasonReact.Update({error: Some(errorMsg)})
        }
    },

    render: ({state, send}) => {

        let onSuccess: (Auth.authUser => unit) = (authUser) => {
            Session.setCurrentUser(Some(authUser));
            Router.navigate(Routes.Character(List));
        };

        let onError: (Api.apiOnError) = (error) => {
            switch (error) {
            | Unauthorised => send(SetError("Login currently unavailable.")) /* Hmmm... server misconfigured? */
            | ApiError(err) => send(SetError(err.summary))
            };
        };

        let onSubmit = (email, password) => {
            Auth.authenticate(email, password, onSuccess, onError);
        };

        <div className="login-wrapper rounded-corners">
            <div className="login-header">
                <img className="login-logo" src=logo />
                <h1 className="center-text">(Elem.string("Please sign in"))</h1>
            </div>

            <LoginForm onSubmit submissionError=?{state.error} />
        </div>
    }
};