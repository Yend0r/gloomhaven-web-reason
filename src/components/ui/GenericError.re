let component = ReasonReact.statelessComponent("GenericError");

let make = (~error, _children) => {
    ...component,
    render: (_self) => {   
        switch (error) {
        | Some(e) => {
                <div className="row justify-content-center pt-1">
                    <div className="pt-2 alert alert-danger">
                        (Elem.string(e)) 
                    </div>
                </div>
            }
        | _ => Elem.nullEl
        };
    }
};