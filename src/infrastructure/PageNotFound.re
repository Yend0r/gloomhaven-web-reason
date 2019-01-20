let component = ReasonReact.statelessComponent("PageNotFound");

let make = (_children) => {
  ...component,
  render: _self =>
    <div>
      (Elem.string("404"))
    </div>
};