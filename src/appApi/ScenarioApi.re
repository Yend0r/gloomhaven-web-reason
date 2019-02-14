type modifierCard = {
    damage       : int,
    drawAnother  : bool, 
    reshuffle    : bool, 
    action       : string, 
    actionAmount : option(int)
};

type modifierDeck = {
    totalCards  : int,
    currentCard : option(modifierCard),
    discards    : list(modifierCard) 
};

type scenario = {
    characterId   : int,
    name          : string,
    health        : int, 
    maxHealth     : int,
    experience    : int,  
    dateStarted   : string,
    dateLastEvent : string,
    modifierDeck  : modifierDeck    
};

let deserializeCard = (json: Js.Json.t) : modifierCard => 
    Json.Decode.{
        damage:       json |> field("damage", int),
        drawAnother:  json |> field("drawAnother", bool),
        reshuffle:    json |> field("reshuffle", bool),
        action:       json |> field("action", string),
        actionAmount: json |> optional(field("actionAmount", int))
    };

let deserializeDeck = (json: Js.Json.t) : modifierDeck => 
    Json.Decode.{
        totalCards:  json |> field("totalCards", int),
        currentCard: json |> optional(field("currentCard", deserializeCard)),
        discards:    json |> field("discards", Json.Decode.list(deserializeCard))
    };

let deserializeScenario = (json: Js.Json.t) : scenario => 
    Json.Decode.{
        characterId:   json |> field("characterId", int),
        name:          json |> field("name", string),
        health:        json |> field("health", int),
        maxHealth:     json |> field("maxHealth", int),
        experience:    json |> field("experience", int),
        dateStarted:   json |> field("dateStarted", string),
        dateLastEvent: json |> field("dateLastEvent", string),
        modifierDeck:  json |> field("modifierDeck", deserializeDeck)
    };

let get = (characterId: int, onSuccess, onError: string => unit) => {
    let onResult = Api.onContentResult(deserializeScenario, onSuccess, onError);
  
    Api.Fetcher.get(Settings.scenarioUrl(characterId), onResult);
};

let newScenario = (characterId: int, name: string, onSuccess, onError) => {
    let postData = Serialization.serializeString("name", name);
    let onResult = Api.onNoContentResult(onSuccess, onError);

    Api.Fetcher.post(Settings.scenarioUrl(characterId), postData, onResult);
};

let completeScenario = (characterId: int, onSuccess, onError) => {
    let onResult = Api.onNoContentResult(onSuccess, onError);

    Api.Fetcher.delete(Settings.scenarioUrl(characterId), onResult);
};

let hpUpdate = (characterId: int, health: int, onSuccess, onError) => {
    let postData = Serialization.serializeInt("health", health);
    let onResult = Api.onContentResult(deserializeScenario, onSuccess, onError);

    Api.Fetcher.patch(Settings.scenarioStatsUrl(characterId), postData, onResult);
};

let xpUpdate = (characterId: int, experience: int, onSuccess, onError) => {
    let postData = Serialization.serializeInt("experience", experience);
    let onResult = Api.onContentResult(deserializeScenario, onSuccess, onError);

    Api.Fetcher.patch(Settings.scenarioStatsUrl(characterId), postData, onResult);
};

let deckAction = (characterId: int, action: string, onSuccess, onError) => {
    let postData = Serialization.serializeString("action", action);
    let onResult = Api.onContentResult(deserializeScenario, onSuccess, onError);

    Api.Fetcher.post(Settings.scenarioDeckUrl(characterId), postData, onResult);
};

let drawCard = (characterId: int, onSuccess, onError) => 
    deckAction(characterId, "DrawCard", onSuccess, onError);

let reshuffle = (characterId: int, onSuccess, onError) => 
    deckAction(characterId, "Reshuffle", onSuccess, onError);