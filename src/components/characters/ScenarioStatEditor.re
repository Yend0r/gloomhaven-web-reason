let component = ReasonReact.statelessComponent("ScenarioStatEditor");

let make = (~label, 
            ~value, 
            ~onDecrement, 
            ~onIncrement, 
            ~onSubmit,
            ~onCancel,
            ~minInclusive=?,
            ~maxInclusive=?, 
            ~showMax=?,
            _children) => {

  ...component,

  render: (_self) => {
        let onDec = (event) => {
            ReactEvent.Mouse.preventDefault(event);
            switch (minInclusive) {
            | Some(min) when value <= min => ()
            | _ => onDecrement()
            };
        };

        let onInc = (event) => {
            ReactEvent.Mouse.preventDefault(event);
            switch (maxInclusive) {
            | Some(max) when value >= max => ()
            | _ => onIncrement()
            };
        };

        let getValueDisplay = () => 
            switch (showMax, maxInclusive) {
            | (Some(show), Some(max)) when show => string_of_int(value) ++ "/" ++ string_of_int(max)
            | (_, _) => string_of_int(value)
            };
    
        <div className="field">
            <label className="label is-size-4">(Elem.string(label))</label>
            <div className="field is-narrow has-addons"> 
                <p className="control">
                    <a className="button is-light" onClick=onDec>
                        (Elem.string("-"))
                    </a>
                </p>
                <div className="control">
                    <p className="property-edit">
                        (Elem.string(getValueDisplay()))
                    </p>
                </div>
                <p className="control">
                    <a className="button is-light" onClick=onInc>
                        (Elem.string("+"))
                    </a>
                </p>
            </div>
            <div className="control">
                <UIButton 
                    title="Save" 
                    onClick=onSubmit 
                    classes="button is-primary m-r-sm" />
                <UIButton 
                        title="Cancel" 
                        onClick=onCancel 
                        classes="button is-light" />
            </div>
        </div>
    }
};