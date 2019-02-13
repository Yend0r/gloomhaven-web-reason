type layout = 
    | Vertical
    | Horizontal;

let component = ReasonReact.statelessComponent("NumberInput");

let make = (~label, 
            ~value, 
            ~onDecrement, 
            ~onIncrement, 
            ~minInclusive=?, 
            ~maxInclusive=?, 
            ~showMax, 
            ~layout,
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
            | (true, Some(max)) => string_of_int(value) ++ "/" ++ string_of_int(max)
            | (_, _) => string_of_int(value)
            };

        let getTextCss = () => {
            if (value == 0) {
                "property-edit"
            } else {
                "property-edit has-text-primary"
            }
        };

        let getLessBtnCss = () => 
            switch (minInclusive) {
            | Some(min) when value <= min => "button is-light"
            | _ => "button is-light"
            };

        let getMoreBtnCss = () => 
            switch (maxInclusive) {
            | Some(max) when value >= max => "button is-light"
            | _ => "button is-light"
            };

        let renderField = () => 
            <div className="field is-narrow has-addons"> /*has-addons-centered*/
                <p className="control">
                    <a className=getLessBtnCss() onClick=onDec>
                        (Elem.string("-"))
                    </a>
                </p>
                <div className="control">
                    <p className=(getTextCss())>
                        (Elem.string(getValueDisplay()))
                    </p>
                </div>
                <p className="control">
                    <a className=getMoreBtnCss() onClick=onInc>
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