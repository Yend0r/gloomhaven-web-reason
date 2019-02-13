let component = ReasonReact.statelessComponent("CharacterPage");

let make = (~url: ReasonReact.Router.url, _children) => {

    ...component,

    render: (_self) => {

        let renderOrDefault = (idStr, renderElement) => {
            switch (Convert.toInt(idStr)) {
                | Some(characterId) => renderElement(characterId)
                | None => <CharacterList />
            };
        };

        switch (url.path) {
        | ["characters", "add"] => 
            <CharacterAdd />
        | ["characters", "edit", idStr] => 
            renderOrDefault(idStr, characterId => <CharacterEdit characterId />)
        | ["characters", "details", idStr] => 
            renderOrDefault(idStr, characterId => <CharacterDetails characterId />)
        | ["characters", "scenario", idStr] => 
            renderOrDefault(idStr, characterId => <ScenarioDetails characterId />)
        | _ => 
            <CharacterList />
        };
    }
};