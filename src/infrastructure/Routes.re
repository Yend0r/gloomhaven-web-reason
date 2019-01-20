type route = 
    | Auth(AuthRoutes.authRoute)
    | Character(CharacterRoutes.characterRoute);

type routeElement = 
    | Public(ReasonReact.reactElement) 
    | AuthRequired(ReasonReact.reactElement);

let defaultRoute = AuthRequired(<CharacterList />);
let notFoundRoute = Public(<PageNotFound />);
let loginRoute = AuthRoutes.Login;