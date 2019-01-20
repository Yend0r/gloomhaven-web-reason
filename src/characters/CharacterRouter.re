let pathHandler = route => 
    switch route {
        | CharacterRoutes.List => "/characters"
        | CharacterRoutes.Details(id) => "/characters/" ++ id
        | CharacterRoutes.Add => "/characters/new"
    };

let charList = Routes.AuthRequired(<CharacterList />);
let charNew = Routes.AuthRequired(<CharacterNew />);
let charDetails = id => Routes.AuthRequired(<CharacterDetails id />);

let routeHandler = (url: ReasonReact.Router.url) => {
    switch url.path {
        | ["characters"] => Some(charList)
        | ["characters", "new"] => Some(charNew)
        | ["characters", idStr] => 
            Convert.toInt(idStr) 
            |> Belt.Option.map(_, id => charDetails(id))
        | _ => None
    };
};