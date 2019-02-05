type layout = 
    | Vertical
    | Horizontal;

let component = ReasonReact.statelessComponent("NumberInput");

let make = (~label, 
            ~value, 
            ~onDecrement, 
            ~onIncrement, 
            ~minInclusive, 
            ~maxInclusive, 
            ~showMax, 
            ~layout,
            _children) => {

    ...component,

  render: (_self) => {
        let onDec = (event) => {
            ReactEvent.Mouse.preventDefault(event);
            if (value - 1 >= minInclusive) {
                onDecrement();
            }
        };

        let onInc = (event) => {
            ReactEvent.Mouse.preventDefault(event);
            if (value + 1 <= maxInclusive) {
                onIncrement();
            }
        };
        
        let getValueDisplay = () => {
            if (showMax) {
                string_of_int(value) ++ "/" ++ string_of_int(maxInclusive)
            } else {
                string_of_int(value)
            }
        };

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
                        (Elem.string("-"))
                    </a>
                </p>
                <div className="control">
                    <p className="property-edit">
                        (Elem.string(getValueDisplay()))
                    </p>
                </div>
                <p className="control">
                    <a className=getBtnCss() onClick=onInc>
                        (Elem.string("+"))
                    </a>
                </p>
            </div>

        switch (layout) {
        | Horizontal => 
            <div className="field is-horizontal">
                <div className="field p-r-md">
                    (renderField())
                </div>
                <label className="label p-t-sm">
                    (Elem.string(label))
                </label>
            </div>
        | Vertical =>
            <div className="field">
                <label className="label">(Elem.string(label))</label>
                (renderField())
            </div>
        }
    }
};