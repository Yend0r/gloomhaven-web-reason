
let serialize = (serializer: 'a => Js.Json.t, data: 'a) : string => {
    data 
    |> serializer
    |> Js.Json.stringify;
};

let deserialize = (deserializer: Js.Json.t => 'a, jsonString: string) : 'a => 
    jsonString 
    |> Js.Json.parseExn 
    |> deserializer;    

let deserializeOption = (deserializer: Js.Json.t => 'a, jsonString: option(string)) : option('a) => 
    switch jsonString {
    | Some(s) => Some(deserialize(deserializer, s))
    | None => None
};