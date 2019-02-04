type claimedPerk = {
    id: string,
    quantity: int,
    actions: string
};

type character = {
    id: int,
    name: string,
    ghClassName: string,
    experience: int,
    level: int,
    hp: int,
    gold: int,
    achievements: int,
    claimedPerks: list(claimedPerk)
};

type characterListItem = {
    id: int,
    name: string,
    ghClassName: string,
    experience: int,
    level: int,
    gold: int
};

type newCharacter = {
    name: string,
    ghClassName: string
};

type selectedPerk = {
    id: string,
    quantity: int
};

type characterUpdate = {
    name: string,
    experience: int,
    gold: int,
    achievements: int,
    perks: list(selectedPerk)
};

let deserializeClaimedPerk = (json: Js.Json.t) : claimedPerk => 
    Json.Decode.{
        id:       json |> field("id", string),
        quantity: json |> field("quantity", int),
        actions:  json |> field("actions", string)
    };

let deserializeCharacter = (json: Js.Json.t) : character => 
    Json.Decode.{
        id:             json |> field("id", int),
        name:           json |> field("name", string),
        ghClassName:    json |> field("className", string),
        experience:     json |> field("experience", int),
        level:          json |> field("level", int),
        hp:             json |> field("hp", int),
        gold:           json |> field("gold", int),
        achievements:   json |> field("achievements", int),
        claimedPerks:   json |> field("claimedPerks", Json.Decode.list(deserializeClaimedPerk))
    };

let deserializeCharacterListItem = (json: Js.Json.t) : characterListItem => 
    Json.Decode.{
        id:          json |> field("id", int),
        name:        json |> field("name", string),
        ghClassName: json |> field("className", string),
        experience:  json |> field("experience", int),
        level:       json |> field("level", int),
        gold:        json |> field("gold", int)
    };

let serializeNewCharacter = (ch: newCharacter) => 
    Json.Encode.(object_([
        ("name", string(ch.name)),
        ("className", string(ch.ghClassName))
    ]));

let serializeSelectedPerk = (perk: selectedPerk) => 
    Json.Encode.(object_([
        ("id", string(perk.id)),
        ("quantity", int(perk.quantity))
    ]));

let serializeCharacterUpdate = (ch: characterUpdate) => 
    Json.Encode.(object_([
        ("name", string(ch.name)),
        ("experience", int(ch.experience)),
        ("gold", int(ch.gold)),
        ("achievements", int(ch.achievements)),
        ("perks", list(serializeSelectedPerk, ch.perks))
    ]));

let getList = (onSuccess, onError: Api.apiOnError) => {
    let onFetchSuccess = (result) => { 
        result
        |> Serialization.deserializeApiListResponse(deserializeCharacterListItem)
        |> onSuccess
    };

    Api.Fetcher.get(Settings.charactersUrl(), onFetchSuccess, onError);
};

let get = (id: int, onSuccess, onError: Api.apiOnError) => {
    let onFetchSuccess = (result) => { 
        result
        |> deserializeCharacter
        |> onSuccess
    };

    Api.Fetcher.get(Settings.characterUrl(id), onFetchSuccess, onError);
};

let add = (newCharacter: newCharacter, onSuccess, onError: Api.apiOnError) => {
    let onAddSuccess = (_result) => onSuccess();
    let postData = newCharacter |> Serialization.serialize(serializeNewCharacter);

    Api.Fetcher.post(Settings.charactersUrl(), postData, onAddSuccess, onError);
};

let update = (characterId: int, characterUpdate: characterUpdate, onSuccess, onError: Api.apiOnError) => {
    let onAddSuccess = (_result) => onSuccess();
    let postData = characterUpdate |> Serialization.serialize(serializeCharacterUpdate);

    Api.Fetcher.post(Settings.characterUrl(characterId), postData, onAddSuccess, onError);
};