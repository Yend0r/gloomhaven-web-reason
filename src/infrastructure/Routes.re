
module AuthRoutes = {
    let rootPath = "auth";

    type route = 
        | Login
        | Logout;

    let pathHandler = route => 
        switch route {
            | Login => "/auth/login"
            | Logout => "/auth/logout"
        };
};

module AccountRoutes = {
    let rootPath = "accounts";

    type route = 
        | ChangePassword;

    let pathHandler = route => 
        switch route {
            | ChangePassword => "/accounts/change-password"
        };
};

module CharacterRoutes = {
    let rootPath = "characters";

    type route = 
        | List
        | Add
        | Edit(int)
        | Details(int);

    let pathHandler = route => 
        switch route {
            | List => "/characters"
            | Add => "/characters/add"
            | Edit(id) => "/characters/edit/" ++ string_of_int(id)
            | Details(id) => "/characters/details/" ++ string_of_int(id)
        };
};

module SpecialRoutes = {
    type route = 
        | Home
        | NotFound;

    let pathHandler = route => 
        switch route {
            | Home => ""
            | NotFound => "/404"
        };
};

type t = 
    | Auth(AuthRoutes.route)
    | Account(AccountRoutes.route)
    | Character(CharacterRoutes.route);

let pathFor = (route: t) : string => 
    switch (route) {
        | Auth(r) => AuthRoutes.pathHandler(r)
        | Account(r) => AccountRoutes.pathHandler(r)
        | Character(r) => CharacterRoutes.pathHandler(r)
    };

let navigate = (route) =>
    route
    |> pathFor
    |> ReasonReact.Router.push