
let apiUrl = "https://127.0.0.1:5001/api/v1/";

let toApiUrl = (restOfUrl) => apiUrl ++ restOfUrl;

let authenticateUrl = () => 
    "authentication/login" 
    |> toApiUrl;

let charactersUrl = () => 
    "characters" 
    |> toApiUrl;

let characterUrl = (characterId) => {
    let id = string_of_int(characterId);
    {j|characters/$(id)|j} 
    |> toApiUrl;
};

let ghClassesUrl = () => 
    "game/classes" 
    |> toApiUrl;
    
let ghClassUrl = (ghClassName) => 
    {j|game/classes/$(ghClassName)|j} 
    |> toApiUrl;

let scenarioUrl = (characterId) => {
    let id = string_of_int(characterId);
    {j|characters/$(id)/scenarios|j} 
    |> toApiUrl;
};

let scenarioStatsUrl = (characterId) => {
    let id = string_of_int(characterId);
    {j|characters/$(id)/scenarios/stats|j} 
    |> toApiUrl;
};

let scenarioDeckUrl = (characterId) => {
    let id = string_of_int(characterId);
    {j|characters/$(id)/scenarios/deck|j} 
    |> toApiUrl;
};