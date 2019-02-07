type perk = {
    id: string,
    quantity: int,
    actions: string
};

type ghClass = {
    ghClassName: string,
    name: string,
    symbol: string,
    isStarting: bool,
    perks: list(perk)
};

let deserializePerk = (json: Js.Json.t) : perk => 
    Json.Decode.{
        id:       json |> field("id", string),
        quantity: json |> field("quantity", int),
        actions:  json |> field("actions", string)
    };

let deserializeGhClass = (json: Js.Json.t) : ghClass => 
    Json.Decode.{
        ghClassName: json |> field("className", string),
        name:        json |> field("name", string),
        symbol:      json |> field("symbol", string),
        isStarting:  json |> field("isStarting", bool),
        perks:       json |> field("perks", Json.Decode.list(deserializePerk))
    };

let getList = (onSuccess, onError: string => unit) => {
    let deserializer = 
        deserializeGhClass
        |>Serialization.deserializeApiListResponse;

    let onResult = Api.onContentResult(deserializer, onSuccess, onError);

    Api.Fetcher.get(Settings.ghClassesUrl(), onResult);
};

let get = (ghClassName, onSuccess, onError: string => unit) => {
    let onResult = Api.onContentResult(deserializeGhClass, onSuccess, onError);

    Api.Fetcher.get(Settings.ghClassUrl(ghClassName), onResult);
};