let toInt = (strVal:string): option(int) => 
    try (strVal |> int_of_string |> (i) => Some(i)) 
    { | _exn => None };

let toStringOpt = (strVal: string): option(int) =>
    try (strVal |> int_of_string |> (i) => Some(i)) 
    { | _exn => None };

let toIntOpt = (strVal: string): option(int) =>
    try (strVal |> int_of_string |> (i) => Some(i)) 
    { | _exn => None };