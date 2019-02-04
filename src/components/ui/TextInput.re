type layout = 
    | Vertical
    | Horizontal;

let component = ReasonReact.statelessComponent("CharDetailEditor");

let make = (
    ~label, 
    ~value, 
    ~placeHolder,
    ~fieldName, 
    ~fieldErrors, 
    ~onChange, 
    ~layout, 
    _children) => {

  ...component,

  render: (_self) => {
        
        let getCss = (fName, fErrors) => {
            let error = FormUtils.tryFindError(fName, fErrors);
            switch(error){
            | Some(_) => "input is-danger"
            | None => "input"
            };
        };

        let renderInput = () => 
            <div className="control">
                <input 
                    className=(getCss(fieldName, fieldErrors))
                    type_="text"
                    id=fieldName
                    placeholder=placeHolder
                    name=fieldName
                    value=value
                    onChange=onChange
                />
            </div>

        switch (layout) {
        | Horizontal => 
            <div className="field is-horizontal">
                <div className="field-label field-label-half is-normal">
                    <label className="label">(Elem.string(label))</label>
                </div>
                <div className="field-body">
                    <div className="field is-narrow">
                        (renderInput())
                    </div>
                </div>
            </div>
        | Vertical =>
            <div className="field">
                <label className="label">(Elem.string(label))</label>
                (renderInput())
            </div>
        }
    }
};