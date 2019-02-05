
let processApiError = (error: Api.apiErrorResult, handler: (string => unit)) => {
    switch (error) {
    | Unauthorised => Session.logout()
    | ApiError(err) => handler(err.message)
    };
}; 