[@bs.module] external logo : string = "../../images/GloomhavenBanner.jpg";

open Routes;

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

        let onSuccess: (AuthApi.authUser => unit) = (authUser) => {
            /* Convert to session user */
            let sessionUser: Session.sessionUser = {
                email: authUser.email,
                name: authUser.name,
                accessToken: authUser.accessToken,
                accessTokenExpiresAt: authUser.expiresAt
            };
            Session.setSessionUser(sessionUser);
            navigate(Character(CharacterRoutes.List));
        };

        let onError = (error) => send(SetError(error));

        let onSubmit = (email, password) => {
            AuthApi.authenticate(email, password, onSuccess, onError);
        };

        <LoginForm onSubmit submissionError=?{state.error} />
    }
};