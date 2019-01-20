type authUser = {
    email: string,
    accessToken: string,
    accessTokenExpiresAt: string
};

let serializeAuthUser = (user: authUser) => 
    Json.Encode.(object_([
        ("email", string(user.email)),
        ("accessToken", string(user.accessToken)),
        ("accessTokenExpiresAt", string(user.accessTokenExpiresAt))
    ]));

let deserializeAuthUser = (json: Js.Json.t) : authUser => 
    Json.Decode.{
        email: field("email", string, json),
        accessToken: field("accessToken", string, json),
        accessTokenExpiresAt: field("accessTokenExpiresAt", string, json)
    };

