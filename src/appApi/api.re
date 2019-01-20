open Json;

/* These two types are used to pre-process the api response */
type apiResponseError = {
    responseBody: string,
    status: int
};

type apiResponse = 
  | OkResponse(string)
  | ErrorResponse(apiResponseError); 

/* These types are used to package the api response nicely for other code */
type errorDetail = {
    title: string,
    detail: string
};
 
type apiError = {
    status: int,
    errors: array(errorDetail),
    summary: string
};

type apiErrorResult = 
    | Unauthorised
    | ApiError(apiError);

type apiOnSuccess = Js.Json.t => unit;
type apiOnError = apiErrorResult => unit;

/* Internal types */
type callResult('a) = 
    | Success('a)
    | Fail(apiErrorResult);

let mapApiErrorJson = (json): errorDetail => {
    title: Decode.field("title", Decode.string, json),
    detail: Decode.field("detail", Decode.string, json)
};

let getErrorSummary = (apiError: array(errorDetail)) => {
    let firstErr = apiError[0];
    if (firstErr.title == firstErr.detail || Utils.stringIsEmpty(firstErr.detail)) {
        firstErr.title
    } else {
        firstErr.title ++ " " ++ firstErr.detail
    };
};

let mapApiErrorResponse = (status, json) => {
    if (status == 401) {
        Unauthorised;
    } else {
        let errors = Decode.field("errors", Decode.array(mapApiErrorJson), json);
        let error = {
            status: status,
            errors: errors,
            summary: getErrorSummary(errors)
        };
        ApiError(error);
    }       
};

module type FetcherType = {
    let get: (string, apiOnSuccess, apiOnError) => unit;
    let post: (string, string, apiOnSuccess, apiOnError) => unit;
};

module Fetcher : FetcherType = {

    let wrapOkPromise = (promise) => 
        Js.Promise.then_((value) => Js.Promise.resolve(OkResponse(value)), promise);

    let wrapErrorPromise = (status, promise) => {
        let err = (value) => ErrorResponse({responseBody: value, status: status});
        Js.Promise.then_((value) => Js.Promise.resolve(err(value)), promise);
    };

    let wrapResponse = (response) => {
        /* Ok = true => status = 200...299 */
        let ok = Bs_fetch.Response.ok(response); 
        switch ok {
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
                Js.Json.parseExn(responseBody) 
                |> (s) => Success(s)
            }
        | ErrorResponse(apiResponseError) => {
                Js.Json.parseExn(apiResponseError.responseBody) 
                |> mapApiErrorResponse(apiResponseError.status)
                |> (s) => Fail(s)
            } 
        } 
    };

    let handleProcessingError = (error) => {
        /* Apparently this will only return "TypeError: failed to fetch"... sigh */
        Js.log(error);
        let err =
            {
                status: 500,
                errors: 
                [|
                    {title: "Unknown Error", detail: "Please try again later."}
                |], 
                summary: "Unknown Error. Please try again later."
            };
        Fail(ApiError(err));
    };

    let handleResult = (onSuccess, onError, result) => {
        switch result {
        | Success(json) => {
            Js.log("API SUCCESS");
            Js.log(json);
            onSuccess(json);
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

    let get = (url, onSuccess: apiOnSuccess, onError: apiOnError) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Get,
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
            ()
        );

        callApi(url, fetchOptions, onSuccess, onError);
    };

    let post = (url, data, onSuccess: apiOnSuccess, onError: apiOnError) => {

        let fetchOptions = Fetch.RequestInit.make(
            ~method_=Post,
            ~body=Fetch.BodyInit.make(data),
            ~credentials=Include,
            ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
            ()
        );
    
        callApi(url, fetchOptions, onSuccess, onError);
    };
};

