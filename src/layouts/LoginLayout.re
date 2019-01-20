[%bs.raw {|require('../css/login.css')|}];

let component = ReasonReact.statelessComponent("LoginLayout");

let make = (~content, _children) => {
    ...component,

    render: (_self) => 
        /* This wold be used for login, forgotten password, etc */
        <div>
            <div className="login-bg"></div>
            <div className="login">
                <div className="login-container">{content}</div>
            </div>
        </div>
};