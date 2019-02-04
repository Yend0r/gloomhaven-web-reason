
type pageLayout = 
    | LoginLayout
    | DefaultLayout;

type routePage = {
    getPage: (ReasonReact.Router.url => ReasonReact.reactElement),
    layout: pageLayout,
    authRequired: bool
};

let createPage = (layout, authRequired, getPage) => {
    layout: layout, 
    getPage: getPage,
    authRequired: authRequired
};

/* These are the "top-level" routes that then have further routing within them */
module Pages = {
    let auth = createPage(LoginLayout, false, (url) => <AuthPage url/>);
    let accounts = createPage(DefaultLayout, true, (url) => <AuthPage url/>);
    let home = createPage(DefaultLayout, true, (url) => <CharacterPage url />);
    let notFound = createPage(DefaultLayout, false, (url) => <AuthPage url />);
    let characters = createPage(DefaultLayout, true, (url) => <CharacterPage url />);
};

module SpecialPages = {
    let loginPage = Pages.auth;
    let homePage = Pages.characters;
};

let matchRoute = (rootPath: string) => {
    Js.log("Router: matching on: " ++ rootPath);
    switch rootPath {
        | ""             => Pages.home
        | "auth" => Pages.auth
        | "accounts"     => Pages.accounts
        | "characters"   => Pages.characters
        | _ => Pages.notFound /* no match => go to 404 */
    };
};

let getPageForRoute = (isLoggedIn, url: ReasonReact.Router.url) => {

    let page = 
        switch (url.path) {
            | [rootPath, ..._rest] => matchRoute(rootPath) 
            | _ => Pages.home /* no path => go to default page */
        };

    /* Check that user is authorised to view content */
    switch (isLoggedIn, page.authRequired) {
        | (false, true) => Pages.auth /* No access... force login */             
        | (_, _) => page 
    };
};

let getInitialUrl = () => 
    ReasonReact.Router.dangerouslyGetInitialUrl();

