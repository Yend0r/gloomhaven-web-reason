
let serialize = (serializer: 'a => Js.Json.t, data: 'a) : string => {
    data 
    |> serializer
    |> Js.Json.stringify;
};

let serializeString = (label: string, value: string) : string => {
    Json.Encode.(object_([(label, string(value))]))
    |> Js.Json.stringify;
};

let serializeInt = (label: string, value: int) : string => {
    Json.Encode.(object_([(label, int(value))]))
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

let deserializeApiListResponse = (deserializeJson : Js.Json.t => 'a, json) => 
    Json.Decode.field("data", Json.Decode.list(deserializeJson), json);