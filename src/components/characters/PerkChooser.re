open CharacterApi;
open GameDataApi;

type chkbxItem = {
    value: string,
    label: string
};

type state = {
    ghClass: GameDataApi.ghClass,
    items : list(chkbxItem),
    selectedItems: list(string),
    selectedPerks: list(CharacterApi.claimedPerk),
    fieldErrors: list(FormUtils.fieldError)
};

let mapToSelectedPerk = (perk: CharacterApi.claimedPerk) : CharacterApi.selectedPerk => {
    id: perk.id, 
    quantity: perk.quantity
};

let addChkbxItem = (idx: int, perk: GameDataApi.perk, items: list(chkbxItem)) => {
    let value = string_of_int(idx) ++ "-" ++ perk.id;
    let item = { value: value, label: perk.actions };
    [item, ... items];
};

let rec perkToChkbx = (idx: int, perk: GameDataApi.perk, items: list(chkbxItem)) => {    
    switch(idx) {
        | 1 => items |> addChkbxItem(1, perk);
        | _ => items |> addChkbxItem(idx, perk) |> perkToChkbx(idx - 1, perk);
    };
};

let perkListToItemList = (perks: list(GameDataApi.perk)) : list(chkbxItem) => {
    perks 
    |> List.fold_left((items, perk) => perkToChkbx(perk.quantity, perk, items), [])
    |> List.rev;
};

let addIdItem = (idx: int, perk: CharacterApi.claimedPerk, items: list(string)) => {
    let id = string_of_int(idx) ++ "-" ++ perk.id;
    [id, ... items];
};

let rec perkToId = (idx: int, perk: CharacterApi.claimedPerk, items: list(string)) => {    
    switch(idx) {
        | 1 => items |> addIdItem(1, perk);
        | _ => items |> addIdItem(idx, perk) |> perkToId(idx - 1, perk);
    };
};

let perkListToIdList = (perks: list(CharacterApi.claimedPerk)) : list(string) => {
    perks 
    |> List.fold_left((items, perk: CharacterApi.claimedPerk) => perkToId(perk.quantity, perk, items), [])
    |> List.rev;
};

type action =
    | SetFieldErrors(list(FormUtils.fieldError))
    | TogglePerk(string);

let component = ReasonReact.reducerComponent("PerkChooser");

let make = (~ghClass: GameDataApi.ghClass,
            ~selectedPerks: list(CharacterApi.claimedPerk),
            ~onChange, 
            ~fieldErrors,
            _children) => {

    ...component,

    initialState: () => { 
        ghClass: ghClass,
        items: perkListToItemList(ghClass.perks),
        selectedItems: perkListToIdList(selectedPerks),
        selectedPerks: selectedPerks,
        fieldErrors: fieldErrors 
    },

    reducer: (action, state) => {
        switch action {
            | SetFieldErrors(errors) => 
                ReasonReact.Update({...state, fieldErrors: errors})
            | TogglePerk(chkbxId) => 
                let isChecked = state.selectedItems |> List.mem(chkbxId);
                let newSelectedItems = 
                    if (isChecked) {
                        state.selectedItems |> List.filter(p => p != chkbxId)
                    } else {
                        [chkbxId, ...state.selectedItems]
                    }
                ReasonReact.Update({...state, selectedItems: newSelectedItems})            
        };
    },

    render: ({state, send}) => {   

        let onTogglePerk = (event) => {

            send(TogglePerk(event |> Utils.valueFromEvent));
        };
        
        let renderItem = (item: chkbxItem, selectedItems: list(string)) => {
            let isChecked = selectedItems |> List.mem(item.value);
            
            <div className="field" key=("chkbx" ++ item.value)>
                <label className="checkbox has-text-weight-bold" key=item.value >
                    <input 
                        type_="checkbox" 
                        value=item.value
                        key=item.value 
                        className="m-r-sm" 
                        checked=isChecked
                        onChange=onTogglePerk/>
                    (Elem.string(item.label)) 
                </label>
            </div>
        };
            
        let renderItems = (items: list(chkbxItem), selectedItems: list(string)) => 
            items
            |> List.map(item => renderItem(item, selectedItems))
            |> Array.of_list
            |> ReasonReact.array;
        
        <div className="field">
            <label className="label">(Elem.string("Perks"))</label>
            (renderItems(state.items, state.selectedItems)) 
        </div>
    }
};
