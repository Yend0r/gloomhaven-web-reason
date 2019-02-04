let component = ReasonReact.statelessComponent("CharacterPage");

let make = (~url: ReasonReact.Router.url, _children) => {

    ...component,

    render: (_self) => {
        switch (url.path) {
        | ["characters", "add"] => <CharacterAdd />
        | ["characters", "edit", idStr] => 
            switch (Convert.toInt(idStr)) {
                | Some(characterId) => <CharacterEdit characterId />
                | None => <CharacterList />
            };
        | ["characters", "details", idStr] => 
            switch (Convert.toInt(idStr)) {
                | Some(characterId) => <CharacterDetails characterId />
                | None => <CharacterList />
            };
        | _ => <CharacterList />
        };
    }
};