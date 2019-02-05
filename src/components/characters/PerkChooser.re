open CharacterApi;

type chkbxItem = {
    value: string,
    label: string
};

type state = {
    ghClass: GameDataApi.ghClass,
    selectedPerks: list(CharacterApi.selectedPerk),
    fieldErrors: list(FormUtils.fieldError)
};

type incDec = 
    | Increment
    | Decrement;

type action =
    | SetFieldErrors(list(FormUtils.fieldError))
    | UpdatePerks(string, incDec);

let component = ReasonReact.reducerComponent("PerkChooser");

let make = (~ghClass: GameDataApi.ghClass,
            ~selectedPerks: list(CharacterApi.selectedPerk),
            ~onChange, 
            ~fieldErrors,
            _children) => {

    ...component,

    initialState: () => { 
        ghClass: ghClass,
        selectedPerks: selectedPerks,
        fieldErrors: fieldErrors 
    },

    reducer: (action, state) => {
        let incDecValue = (incDec, value) => 
            switch (incDec) {
            | Increment => value + 1;
            | Decrement => (value <= 0) ? 0 : value - 1;
            };

        switch action {
            | SetFieldErrors(errors) => 
                ReasonReact.Update({...state, fieldErrors: errors})
            | UpdatePerks(perkId, incDec) => {
                let selectedPerk = state.selectedPerks |> Utils.listTryFind(p => p.id == perkId);

                let newPerks = 
                    switch(selectedPerk){
                        | None => [{id: perkId, quantity: 1},...state.selectedPerks]
                        | Some(_) => {
                            state.selectedPerks
                            |> List.map(p => 
                                switch (p.id == perkId) {
                                    | true => {id: perkId, quantity: incDecValue(incDec, p.quantity)}
                                    | false => p
                                }
                            )
                        }
                    };

                ReasonReact.UpdateWithSideEffects(
                    {...state, selectedPerks: newPerks}, 
                    self => onChange(self.state.selectedPerks)
                );
            }
        };
    },

    render: ({state, send}) => {   
        
        let onDecPerk = (perkId) => send(UpdatePerks(perkId, Decrement));
        let onIncPerk = (perkId) => send(UpdatePerks(perkId, Increment));

        let renderPerk = (perk: GameDataApi.perk, selectedPerks: list(CharacterApi.selectedPerk)) => {

            /* This match fn is here to help the compiler */
            let matchPerkId = (perk: GameDataApi.perk, selectedPerk: CharacterApi.selectedPerk) =>
                perk.id == selectedPerk.id;

            let selectedPerk = selectedPerks |> Utils.listTryFind(p => matchPerkId(perk, p));

            let currentQty = Belt.Option.mapWithDefault(selectedPerk, 0, p => p.quantity);
            
            <div key=("perkQty-"++perk.id)>
                <NumberInput label=perk.actions
                    value=currentQty 
                    onDecrement=(() => onDecPerk(perk.id))
                    onIncrement=(() => onIncPerk(perk.id))
                    minInclusive=0 
                    maxInclusive=perk.quantity
                    showMax=true 
                    layout=Horizontal />
            </div>
        };
            
        let renderPerks = (perks: list(GameDataApi.perk), selectedPerks: list(CharacterApi.selectedPerk)) => 
            perks
            |> List.map(perk => renderPerk(perk, selectedPerks))
            |> Array.of_list
            |> ReasonReact.array;
        
        <div className="field">
            <label className="label">(Elem.string("Perks"))</label>
            (renderPerks(state.ghClass.perks, state.selectedPerks)) 
        </div>
    }
};
