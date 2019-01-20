let component = ReasonReact.statelessComponent("CharacterList");

let make = (_children) => {
  ...component,
  render: _self =>
    <div>
      {Elem.string("Character List")}
    </div>
};