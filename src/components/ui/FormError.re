let component = ReasonReact.statelessComponent("FormError");

let make = (~validationError, ~submissionError, _children) => {
    ...component,
    render: (_self) => {   
        let error = Utils.combineStringOptions(validationError, submissionError);
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