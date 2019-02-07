type authUser = {
    email: string,
    name: string,
    accessToken: string,
    expiresAt: string
};

type loginRequest = {
    email: string,
    password: string
};

let deserializeAuthUser = (json: Js.Json.t) : authUser => 
    Json.Decode.{
        email:       json |> field("email", string),
        name:        json |> field("name", string),
        accessToken: json |> field("accessToken", string),
        expiresAt:   json |> field("accessTokenExpiresAt", string)
    };

let serializeLoginRequest = (req: loginRequest) => 
    Json.Encode.(object_([
        ("email", string(req.email)),
        ("password", string(req.password))
    ]));

let authenticate = (email: string, password: string, onSuccess, onError): unit => {
    
    let onResult = Api.onContentResult(deserializeAuthUser, onSuccess, onError);

    let postData = 
        { email: email, password: password } 
        |> Serialization.serialize(serializeLoginRequest);

    Api.Fetcher.post(Settings.authenticateUrl(), postData, onResult);
};