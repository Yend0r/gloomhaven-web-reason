
type state = {
    character: CharacterApi.character,
    ghClass: GameDataApi.ghClass,
    name: string,
    experience: int,
    gold: int,
    achievements: int,
    selectedPerks: list(CharacterApi.claimedPerk),
    fieldErrors: list(FormUtils.fieldError)
};

let mapToSelectedPerk = (perk: CharacterApi.claimedPerk) : CharacterApi.selectedPerk => {
    id: perk.id, 
    quantity: perk.quantity
};

type incDec = 
    | Increment
    | Decrement;

type action =
    | SetFieldErrors(list(FormUtils.fieldError))
    | ClearErrors
    | ChangeName(string)
    | UpdateGold(incDec)
    | UpdateXp(incDec)
    | UpdateAchievements(incDec);

let component = ReasonReact.reducerComponent("CharacterEditForm");

let make = (~character: CharacterApi.character, 
            ~ghClass: GameDataApi.ghClass,
            ~onSubmit: CharacterApi.characterUpdate => unit, 
            ~onCancel, 
            ~submissionError=?, 
            _children) => {

    ...component,

    initialState: () => { 
        character: character,  
        ghClass: ghClass,
        name: character.name,
        experience: character.experience,
        gold: character.gold,
        achievements: character.achievements,
        selectedPerks: character.claimedPerks,
        fieldErrors: [] 
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
            | ClearErrors => 
                ReasonReact.Update({...state, fieldErrors: []})
            | ChangeName(text) => 
                ReasonReact.Update({...state, name: text})
            | UpdateGold(incDec) => {
                let gold = incDecValue(incDec, state.gold);
                ReasonReact.Update({...state, gold: gold});
            }
            | UpdateXp(incDec) => {
                let xp = incDecValue(incDec, state.experience);
                ReasonReact.Update({...state, experience: xp});
            }
            | UpdateAchievements(incDec) => {
                let achv = incDecValue(incDec, state.achievements);
                ReasonReact.Update({...state, achievements: achv});
            }
        };
    },

    render: ({state, send}) => {   

        let onChangeName = (event) => send(ChangeName(event |> Utils.valueFromEvent));
        let onDecGold = () => send(UpdateGold(Decrement));
        let onIncGold = () => send(UpdateGold(Increment));

        let onDecXp = () => send(UpdateXp(Decrement));
        let onIncXp = () => send(UpdateXp(Increment));

        let onDecAchv = () => send(UpdateAchievements(Decrement));
        let onIncAchv = () => send(UpdateAchievements(Increment));
        
        let onChangePerk = (perkId, quantity) => (); 

        let submit = () => {
            let characterUpdate: CharacterApi.characterUpdate = {
                name: state.name,
                experience: state.experience,
                gold: state.gold,
                achievements: state.achievements,
                perks: []
            };
            onSubmit(characterUpdate);
        };

        let validate: unit => unit = () => {
            send(ClearErrors);
            
            let fieldErrors = 
                FormUtils.validateRequiredString(state.name, "name", "Character name is required.", []);

            switch(fieldErrors) {
            | [] => submit()
            | _ => send(SetFieldErrors(fieldErrors))
            };              
        };

        let cancel = () => 
            onCancel(); 
        
        <form>
            <FormError validationError={None} submissionError />
 
            <TextInput
                label="Name" 
                value=state.name
                placeHolder="Name" 
                fieldName="name"
                fieldErrors=state.fieldErrors
                onChange=onChangeName 
                layout=Vertical />

            <CharDetailEditor 
                label="Gold Earned"
                value=state.gold
                onDecrement=onDecGold
                onIncrement=onIncGold
                layout=Vertical />
            
            <CharDetailEditor  
                label="Experience"
                value=state.experience
                onDecrement=onDecXp
                onIncrement=onIncXp
                layout=Vertical />
                               
            <CharDetailEditor  
                label="Achievements"
                value=state.achievements
                onDecrement=onDecAchv
                onIncrement=onIncAchv
                layout=Vertical />
      
            <PerkChooser
                ghClass=state.ghClass
                selectedPerks=state.selectedPerks
                onChange=onChangePerk
                fieldErrors=[] />
       
            <div className="field is-grouped m-t-lg">
                <div className="control">
                    <UIButton 
                        title="Submit" 
                        onClick=validate 
                        classes="button is-primary" />
                </div>
                <div className="control">
                    <UIButton 
                        title="Cancel" 
                        onClick=cancel 
                        classes="button is-light" />
                </div>
            </div>
        </form>
    }
};