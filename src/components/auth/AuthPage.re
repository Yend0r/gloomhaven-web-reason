let component = ReasonReact.statelessComponent("AuthPage");

let make = (~url: ReasonReact.Router.url, _children) => {

    ...component,

    render: (_self) => {

        switch (url.path) {
        | [_, "password-reset"] => <Login />
        | _ => <Login />
        };
    }
};