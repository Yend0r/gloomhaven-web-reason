[%bs.raw {|require('../css/loginLayout.css')|}];
[%bs.raw {|require('../css/layout.css')|}];

let component = ReasonReact.statelessComponent("LoginLayout");

let make = (~page, _children) => {
    ...component,

    render: (_self) => 
        /* This is be used for login, forgotten password, etc */
        <div className="hero is-success is-fullheight">
            <div className="hero-body">
                {page}
            </div>
        </div> 
};