let component = ReasonReact.statelessComponent("CharacterDetails");

let make = (~id: int, _children) => {
  ...component,
  render: _self =>
    <div>
      {Elem.string("Character Details for ")}
      {Elem.int(id)}
    </div>
};