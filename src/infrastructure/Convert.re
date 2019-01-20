let toInt = (value:string): option(int) => {
    try (
        value 
        |> int_of_string 
        |> (i) => Some(i)
    ) 
    { | _exn => None };
};