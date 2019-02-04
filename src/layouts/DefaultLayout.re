[%bs.raw {|require('../css/defaultLayout.css')|}];
[%bs.raw {|require('../css/layout.css')|}];
[%bs.raw {|require('../css/main.css')|}];

let component = ReasonReact.statelessComponent("DefaultLayout");

let make = (~page, _children) => {

    ...component,

    render: (_self) => 
        (ReasonReact.array([|
            <Navbar key="navbar"/>,
            <div className="container p-l-md" key="container">
                {page}
            </div>
        |]))
};
