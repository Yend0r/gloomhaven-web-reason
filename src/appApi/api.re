open Json;

/* These two types are used to pre-process the api response */
type apiResponseError = {
    jsonError: string,
    status: int
};

type apiResponse = 
  | OkResponse(string)
  | ErrorResponse(apiResponseError); 

/* These types are used to package the api response nicely for other code */
type apiError = {
    status: int,
    message: string,
    detail: string
};

type apiErrorResult = 
    | Unauthorised
    | ApiError(apiError);

type apiOnGetSuccess = Js.Json.t => unit;

type apiOnPostSuccess = 
    | OnJson(Js.Json.t => unit)
    | On204(unit => unit);

type apiOnError = apiErrorResult => unit;

/* Internal types */
type callResult('a) = 
    | Success('a)
    | Fail(apiErrorResult);

let mapApiErrorJson = (status, json): apiError => {
    status: status,
    message: Decode.field("message", Decode.string, json),
    detail: Decode.field("detail", Decode.string, json)
};

let mapApiErrorResponse = (status, json) => {
    if (status == 401) {
        Unauthorised;
    } else {
        let error = mapApiErrorJson(status, json);
        ApiError(error);
    }       
};

module type FetcherType = {
    let get: (string, apiOnGetSuccess, apiOnError) => unit;
    let post: (string, string, apiOnPostSuccess, apiOnError) => unit;
};

module Fetcher : FetcherType = {

    let wrapOkPromise = (promise) => 
        Js.Promise.then_((value) => Js.Promise.resolve(OkResponse(value)), promise);

    let wrapErrorPromise = (status, promise) => {
        let err = (value) => ErrorResponse({jsonError: value, status: status});
        Js.Promise.then_((value) => Js.Promise.resolve(err(value)), promise);
    };

    let wrapResponse = (response) => {
        /* Ok = true => status = 200...299 */
        let ok = Bs_fetch.Response.ok(response); 
        switch (ok) {
        | true => {                
                Bs_fetch.Response.text(response)
                |> wrapOkPromise;
            }
        | false => {
                let status = Bs_fetch.Response.status(response);
                Bs_fetch.Response.text(response)
                |> wrapErrorPromise(status);
            }
        };
    };
  
    let handleResponse = (result) => {
        switch result {
        | OkResponse(responseBody) => {
                let json = 
                    if (Utils.stringIsEmpty(responseBody)) {
                        /* This is terrible... fix it */
                        "{\"Response\":\"204 NoContent\"}";
                    } else {
                        responseBody
                    };
                Js.Json.parseExn(json) 
                |> (s) => Success(s)
            }
        | ErrorResponse(apiResponseError) => {
                Js.Json.parseExn(apiResponseError.jsonError) 
                |> mapApiErrorResponse(apiResponseError.status)
                |> (s) => Fail(s)
            } 
        } 
    };

    let handleProcessingError = (error) => {
        /* Apparently this will only return "TypeError: failed to fetch"... 
           so the actual error cannot be retrieved... sigh */
        Js.log(error);
        let err =
            {
                status: 500,
                message: "Unknown Error. Please try again later.",
                detail: "Unknown Client Error."
            };
        Fail(ApiError(err));
    };

    let handleResult = (onSuccess, onError, result) => {
        switch result {
        | Success(json) => {
            Js.log("API SUCCESS");
            Js.log(json);
            switch(onSuccess){
            | OnJson(processJson) => processJson(json);
            | On204(process204) => process204();
            };
        }
        | Fail(error) => onError(error)
        } 
    };

    let callApi = (url, fetchOptions, onSuccess, onError) => {
        let processResult = handleResult(onSuccess, onError);

        Js.Promise.(
            Bs_fetch.fetchWithInit(url, fetchOptions) 
            |> then_(wrapResponse)
            |> then_((result) => {
                Js.log("API FUN");
                Js.log(result);
                handleResponse(result) |> processResult |> ignore;
                resolve(None);
            })
            |> catch((ex) => {
                Js.log("API NOT FUN");
                Js.log(ex);
                handleProcessingError(ex) |> processResult |> ignore;
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

    let get = (url, onSuccess: apiOnGetSuccess, onError: apiOnError) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Get,
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.makeWithDict(getHeaders()),
            ()
        );

        callApi(url, fetchOptions, OnJson(onSuccess), onError);
    };

    let post = (url, data, onSuccess: apiOnPostSuccess, onError: apiOnError) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Post,
            ~body=Fetch.BodyInit.make(data),
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.makeWithDict(getHeaders()),
            ()
        );
    
        callApi(url, fetchOptions, onSuccess, onError);
    };
};

