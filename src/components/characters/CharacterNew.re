let component = ReasonReact.statelessComponent("CharacterNew");

let make = (_children) => {
  ...component,
  render: _self =>
    <div>
      {Elem.string("Character New for ")}
    </div>
};