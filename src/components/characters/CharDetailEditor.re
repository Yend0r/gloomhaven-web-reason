type layout = 
    | Vertical
    | Horizontal;

let component = ReasonReact.statelessComponent("CharDetailEditor");

let make = (~label, ~value, ~onDecrement, ~onIncrement, ~layout, _children) => {
  ...component,

  render: (_self) => {
        let onDec = Utils.handleBtnClick(onDecrement);
        let onInc = Utils.handleBtnClick(onIncrement);
        
        let getBtnCss = () => {
            if (value == 0) {
                "button is-light"
            } else {
                "button is-primary"
            }
        };

        let renderField = () => 
            <div className="field is-narrow has-addons"> /*has-addons-centered*/
                <p className="control">
                    <a className=getBtnCss() onClick=onDec>
                        (Elem.string("Less"))
                    </a>
                </p>
                <div className="control">
                    <p className="property-edit">
                        (Elem.string(string_of_int(value)))
                    </p>
                </div>
                <p className="control">
                    <a className=getBtnCss() onClick=onInc>
                        (Elem.string("More"))
                    </a>
                </p>
            </div>

        switch (layout) {
        | Horizontal => 
            <div className="field is-horizontal">
                <div className="field-label field-label-half is-normal">
                    <label className="label">(Elem.string(label))</label>
                </div>
                <div className="field-body">
                    (renderField())
                </div>
            </div>
        | Vertical =>
            <div className="field">
                <label className="label">(Elem.string(label))</label>
                (renderField())
            </div>
        }
    }
};