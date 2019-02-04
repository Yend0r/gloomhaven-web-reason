let component = ReasonReact.statelessComponent("UIButton");

let make = (~title, ~onClick, ~classes, ~key: option(string)=?, _children) => {
  ...component,

  render: (_self) => {
        let onClickFn = Utils.handleBtnClick(onClick);

        let btn = () => {
            <button 
                className={classes} 
                onClick={onClickFn}>
                (Elem.string(title))
            </button>
        };

        let btnWithKey = (key: string) => {
            <button 
                className={classes}
                key={key}
                onClick={onClickFn}>
                (Elem.string(title))
            </button>
        };

        switch (key) {
        | Some(k) => btnWithKey(k)
        | None => btn()
        };        
    }
};