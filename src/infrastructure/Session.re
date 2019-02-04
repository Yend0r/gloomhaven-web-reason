type sessionUser = {
    email: string,
    name: string,
    accessToken: string,
    accessTokenExpiresAt: string
};

let serializeUser = (user: sessionUser) => 
    Json.Encode.(object_([
        ("email", string(user.email)),
        ("name", string(user.name)),
        ("accessToken", string(user.accessToken)),
        ("accessTokenExpiresAt", string(user.accessTokenExpiresAt))
    ]));

let deserializeUser = (json: Js.Json.t) : sessionUser => 
    Json.Decode.{
        email: field("email", string, json),
        name: field("name", string, json),
        accessToken: field("accessToken", string, json),
        accessTokenExpiresAt: field("accessTokenExpiresAt", string, json)
    };

let setSessionItem = (key, value) => Dom_storage.setItem(key, value, Dom_storage.sessionStorage);

let getSessionItem = (key) => Dom_storage.getItem(key, Dom_storage.sessionStorage);

let clearSession = () => Dom_storage.clear(Dom_storage.sessionStorage);

let storedUserKey = "GloomChars.sessionUser";

let getStoredUser = (): option(sessionUser) => {
    try (
        getSessionItem(storedUserKey)
        |> Serialization.deserializeOption(deserializeUser)
    ) 
    { 
        | _exn => {
            /* User in session failed to deserialize, so clear it... */
            clearSession(); 
            None;
        }
    }
};

/* 
 This uses a mutable variable (by putting it into a ref() wrapper that is 
 like a box with mutable contents). This is to minimise the number of times 
 that Dom storage is accessed and the subsequent json parsing is performed. 
 Note: The "getCurrentUser()" method is only called once the first time currentUser is accessed.
 */
let storedUser: ref(option(sessionUser)) = ref(getStoredUser());

let getCurrentUser = () => storedUser^;

let isLoggedIn = () => Belt.Option.isSome(storedUser^);

let setSessionUser = (user: sessionUser) => {
    user
    |> Serialization.serialize(serializeUser)
    |> setSessionItem(storedUserKey);
    
    storedUser := Some(user); /* Mutating here for performance reasons */
};

let logout = () => {
    storedUser := None; /* Mutating here for performance reasons */
    clearSession(); 
    Routes.navigate(Routes.Auth(Login));
};