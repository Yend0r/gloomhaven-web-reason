
let loginElement = <Login />;

let pathHandler = action => 
    switch action {
        | AuthRoutes.Login => "/login"
        | AuthRoutes.Logout => "/logout/"
        | AuthRoutes.ChangePassword => "/change-password"
    };

let authLogin = Routes.Public(<Login />);
let authLogout = Routes.AuthRequired(<Logout />);
let authChangePwd = Routes.AuthRequired(<ChangePassword />);

let routeHandler = action => 
    switch action {
        | AuthRoutes.Login => authLogin
        | AuthRoutes.Logout => authLogout
        | AuthRoutes.ChangePassword => authChangePwd
    };

let urlHandler = (url: ReasonReact.Router.url) => {
    switch url.path {
        | ["login"] => Some(authLogin)
        | ["logout"] => Some(authLogout)        
        | ["change-password"] => Some(authChangePwd)
        | _ => None
    };
};