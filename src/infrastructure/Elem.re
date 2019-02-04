let string = (value: string) => 
    ReasonReact.string(value);

let int = (value: int) => 
    value 
    |> string_of_int 
    |> string;

let list = (f: 'a => ReasonReact.reactElement, items:list('a)): ReasonReact.reactElement =>
    items
    |> List.map(f)
    |> Array.of_list
    |> ReasonReact.array;

let nullEl = ReasonReact.null;

let array = ReasonReact.array;