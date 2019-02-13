open Json;

type apiError = {
    status:  int,
    message: string,
    detail:  string
};

type callResult = 
    | Ok(string)
    | NoContent
    | Unauthorised
    | Error(apiError);

/* Helper to build a result handler for the clients */
let onContentResult = (deserializer, onSuccess, onError) => (callResult) => {
    let onContentSuccess = (result) => 
        result 
        |> Js.Json.parseExn
        |> deserializer 
        |> onSuccess;

    switch (callResult: callResult) {
        | NoContent => onError("Error. No data returned from server.")
        | Ok(result) => onContentSuccess(result)
        | Unauthorised => Session.logout() 
        | Error(err) => onError(err.message)
    };
};

/* Helper to build a result handler for the clients */
let onNoContentResult = (onSuccess, onError) => (callResult) => 
    switch (callResult: callResult) {
        | NoContent => onSuccess()
        | Ok(_) => onSuccess()
        | Unauthorised => Session.logout() 
        | Error(err) => onError(err.message)
    };

let mapApiErrorJson = (status, json): apiError => {
    status: status,
    message: Decode.field("message", Decode.string, json),
    detail: Decode.field("detail", Decode.string, json)
};

let toCallResultError = (status, jsonString) => 
    Js.Json.parseExn(jsonString) 
    |> mapApiErrorJson(status)
    |> (e) => Error(e);

let debug = (callResult) => {
    if (1 == 1) {
        switch (callResult) {
            | NoContent => Js.log("API SUCCESS: NO CONTENT")
            | Ok(result) => {
                Js.log("API SUCCESS");
                Js.log(result);
            }
            | Unauthorised => Js.log("API ERROR: Unauthorised")
            | Error(err) => {
                Js.log("API ERROR");
                Js.log(err);
            }
        }
    }
};

module type FetcherType = {
    let get: (string, callResult => unit) => unit;
    let post: (string, string, callResult => unit) => unit;
    let patch: (string, string, callResult => unit) => unit;
};

module Fetcher : FetcherType = {
  
    let handleProcessingError = (ex) : callResult => {
        /* Apparently this will only return "TypeError: failed to fetch"... 
           so the actual error cannot be retrieved... sigh */
        Js.log(ex);
        let err =
            {
                status: 500,
                message: "Unknown Error. Please try again later.",
                detail: "Unknown Client Error."
            };
        Error(err);
    };

    let callApi = (url, fetchOptions, onResult) => {
        Js.Promise.(
            Bs_fetch.fetchWithInit(url, fetchOptions) 
            |> then_((response) => {
                let ok = Bs_fetch.Response.ok(response); 
                let status = Bs_fetch.Response.status(response);

                Bs_fetch.Response.text(response)
                |> then_((result) => {
                    let apiResult : callResult = 
                        switch (ok, status) {
                            | (true,204)  => NoContent
                            | (true,_)    => Ok(result)
                            | (false,401) => Unauthorised
                            | (false,_)   => toCallResultError(status, result) 
                        };
                    debug(apiResult);
                    onResult(apiResult) |> ignore;
                    resolve(None);
                })
            })
            |> catch((ex) => {
                handleProcessingError(ex) |> onResult;
                resolve(None);
            })
        ) |> ignore;
    };

    let contentTypeHeader = (headers) => {
        [("Content-Type", "application/json"), ...headers];
    };

    let authorizationHeader = (headers) => {
        switch (Session.getCurrentUser()) {
        | Some(user) => [("Authorization", user.accessToken), ...headers]
        | None => headers
        };
    };

    let getHeaders = () => {
        []
        |> contentTypeHeader
        |> authorizationHeader
        |> Js.Dict.fromList
    };

    let get = (url, onResult: callResult => unit) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Get,
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.makeWithDict(getHeaders()),
            ()
        );

        callApi(url, fetchOptions, onResult);
    };

    let post = (url, data, onResult: callResult => unit) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Post,
            ~body=Fetch.BodyInit.make(data),
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.makeWithDict(getHeaders()),
            ()
        );
    
        callApi(url, fetchOptions, onResult);
    };

    let patch = (url, data, onResult: callResult => unit) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Patch,
            ~body=Fetch.BodyInit.make(data),
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.makeWithDict(getHeaders()),
            ()
        );
    
        callApi(url, fetchOptions, onResult);
    };
};

