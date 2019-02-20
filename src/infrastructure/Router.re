
type pageLayout = 
    | LoginLayout
    | DefaultLayout;

type routePage = {
    renderPage: (unit => ReasonReact.reactElement),
    layout: pageLayout,
    authRequired: bool
};

let createPage = (layout, authRequired, renderPage) => {
    layout: layout, 
    renderPage: renderPage,
    authRequired: authRequired
};

let createDefaultPage = (elem) => {
    layout: DefaultLayout, 
    renderPage: () => elem,
    authRequired: true
};

let createLoginPage = (elem) => {
    layout: LoginLayout, 
    renderPage: () => elem,
    authRequired: false
};

let renderOrDefault = (idStr, renderElement, defaultElement) => 
    switch (Convert.toIntOpt(idStr)) {
        | Some(id) => createDefaultPage(renderElement(id))
        | None => createDefaultPage(defaultElement)
    };    

module SpecialPages = {
    let loginPage = createLoginPage(<Login />);
    let homePage = createDefaultPage(<CharacterList />);
    let notFoundPage = createLoginPage(<PageNotFound />);
};

let authRoute = (url: ReasonReact.Router.url) => 
    switch (url.path) {  
        | ["auth", "login"] => 
            createLoginPage(<Login />)
        | ["auth", "password-reset"] => 
            createLoginPage(<Login />) /* pwd reset not implemented yet */
        | _ => 
            SpecialPages.notFoundPage
    };

let accountsRoute = (url: ReasonReact.Router.url) => 
    switch (url.path) {
        | ["accounts", "profile"] => 
            createDefaultPage(<Profile />)
        | _ => 
            SpecialPages.notFoundPage
    };

let characterRoute = (url: ReasonReact.Router.url) => {
    let renderCh = (idStr, renderElement) =>
        renderOrDefault(idStr, renderElement, <CharacterList />);
    
    switch (url.path) {
        | ["characters"] => 
            createDefaultPage(<CharacterList />)
        | ["characters", "add"] => 
            createDefaultPage(<CharacterAdd />)
        | ["characters", "edit", idStr] => 
            renderCh(idStr, characterId => <CharacterEdit characterId />)
        | ["characters", "details", idStr] => 
            renderCh(idStr, characterId => <CharacterDetails characterId />)
        | ["characters", "scenario", idStr] => 
            renderCh(idStr, characterId => <ScenarioDetails characterId />)
        | ["characters", "new-scenario", idStr] => 
            renderCh(idStr, characterId => <NewScenario characterId />)
        | _ => 
            SpecialPages.notFoundPage
    };
};

let getPageForRoute = (isLoggedIn, url: ReasonReact.Router.url) => {

    Debug.log("Router matching on: ");
    Debug.log(url);

    let page = 
        switch (url.path) {
            | ["", ..._] => SpecialPages.homePage
            | ["auth", ..._] => authRoute(url) 
            | ["accounts", ..._] => authRoute(url) 
            | ["characters", ..._] => characterRoute(url) 
            | ["scenarios", ..._] => characterRoute(url) 
            | _ => SpecialPages.notFoundPage
        };

    /* Check if login is required to view page */
    switch (isLoggedIn, page.authRequired) {
        | (false, true) => SpecialPages.loginPage /* No access... force login */             
        | (_, _) => page 
    };
};

let getInitialUrl = () => 
    ReasonReact.Router.dangerouslyGetInitialUrl();

