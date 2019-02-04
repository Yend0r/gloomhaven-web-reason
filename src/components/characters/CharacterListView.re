
let component = ReasonReact.statelessComponent("CharacterListView");

let make = (~data: list(CharacterApi.characterListItem), _children) => {
    ...component,

    render: (_self) => {   

        let gotoAdd = () => 
            Routes.navigate(Routes.Character(Add));
            
        let gotoDetails = (id: int) => 
            () => Routes.navigate(Routes.Character(Details(id)));

        let gotoEdit = (id: int) => 
            () => Routes.navigate(Routes.Character(Edit(id)));

        let renderTR = (label, value) => 
            <tr>
                <td className="has-text-weight-bold">(Elem.string(label))</td>
                <td className="has-text-weight-bold">(Elem.string(value))</td>
            </tr>

        let renderChar = (idx, item: CharacterApi.characterListItem) => 
            <div 
                className="column is-5 m-r-xl" 
                key={"ch" ++ string_of_int(idx)} >
                 <div className="box message is-primary p-none">
                    <div className="message-body">
                        <h4 className="title is-size-4">(Elem.string(item.name))</h4> 
                        <table className="table is-fullwidth">
                            <tbody>
                                (renderTR("Class", item.ghClassName))
                                (renderTR("Level", string_of_int(item.level)))
                                (renderTR("XP", string_of_int(item.experience)))
                                (renderTR("Gold", string_of_int(item.gold)))      
                            </tbody>
                        </table>

                        <UIButton 
                            title="Play Character" 
                            onClick=gotoDetails(item.id) 
                            classes="button is-primary m-r-md" />

                        <UIButton 
                            title="Edit Character" 
                            onClick=gotoEdit(item.id) 
                            classes="button is-link" />
                    </div>
                </div>
            </div>

        let renderChars = (data : list(CharacterApi.characterListItem)) => 
            List.mapi((index, item) => renderChar(index, item), data)
            |> Array.of_list
            |> ReasonReact.array;

        <div>
            <div className="title">(Elem.string("Characters"))</div>
            <div className="columns flex-wrap-wrap">
                {renderChars(data)}
            </div>
            <div>
                <UIButton 
                    title="Add Character" 
                    onClick=gotoAdd 
                    classes="button is-link" />
            </div>
        </div>

    }
}; 
