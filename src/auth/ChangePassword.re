let component = ReasonReact.statelessComponent("ChangePassword");

let make = (_children) => {
  ...component,
  render: _self =>
    <div>
      {Elem.string("ChangePassword")}
    </div>
};