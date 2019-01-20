let setSessionItem = (key, value) => Dom_storage.setItem(key, value, Dom_storage.sessionStorage);

let getSessionItem = (key) => Dom_storage.getItem(key, Dom_storage.sessionStorage);

let clearSession = () => Dom_storage.clear(Dom_storage.sessionStorage);

let getCurrentUser = (): option(Auth.authUser) => {
    getSessionItem("authUser")
    |> Serialization.deserializeOption(Auth.deserializeAuthUser);
};

/* 
 This uses a mutable variable (by putting it into a ref() wrapper that is 
 like a box with mutable contents). This is to minimise the number of times 
 that Dom storage is accessed and the subsequent json parsing is performed. 
 Note: The "getCurrentUser()" method is only called the first time currentUser is accessed.
 */
let currentUser: ref(option(Auth.authUser)) = ref(getCurrentUser());

let setCurrentUser = (user: option(Auth.authUser)) => {
    switch user {
    | Some(authUser) => {
            authUser
            |> Serialization.serialize(Auth.serializeAuthUser)
            |> setSessionItem("authUser");
        }
    | None => clearSession();
    };
    currentUser := user; /* Mutating here for performance reasons */
};

let isLoggedIn = () => Belt.Option.isSome(currentUser^);

let logout = () => {
    setCurrentUser(None); 
};