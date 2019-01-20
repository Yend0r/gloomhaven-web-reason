
let defaultHandler = (url: ReasonReact.Router.url) => {
    switch url.path {
        | [] => Some(Routes.defaultRoute)
        | _  => None
    };
};

let routeHandlers = () => [
    defaultHandler,
    AuthRouter.routeHandler,
    CharacterRouter.routeHandler
];

let checkLogin = (isLoggedIn: bool, route) => 
    isLoggedIn ? route : Routes.Auth(Login);

let resolve = (isLoggedIn: bool, url: ReasonReact.Router.url) => {

    let routeElms = 
        routeHandlers()
        |> List.map(handler => handler(url))
        |> List.filter(Belt.Option.isSome);

    let routeElm = 
        switch routeElms {
            | [Some(elm), ..._rest] => elm
            | _ => Routes.notFoundRoute
        };

    switch (isLoggedIn, routeElm) {
        | (true, Routes.AuthRequired(elm)) => elm
        | (false, Routes.AuthRequired(_)) => Routes.loginElement 
        | (_, Routes.Public(elm)) => elm
    };
};

let pathFor = (route: Routes.route) : string => 
    switch (route) 
    {
        | Auth(action) => AuthRouter.pathHandler(action)
        | Character(action) => CharacterRouter.pathHandler(action)
    };

let navigate = (route: Routes.route) => 
    route
    |> pathFor
    |> ReasonReact.Router.push;