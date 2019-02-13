open CharacterApi;

type state = {
    character: CharacterApi.character,
    scenario: ScenarioApi.scenario,
    hpModalVisible: bool,
    xpModalVisible: bool
};

type incDec = 
    | Increment
    | Decrement;

type action =
    | UpdateHp(incDec)
    | UpdateXp(incDec)
    | ToggleHpModal
    | ToggleXpModal;

let component = ReasonReact.reducerComponent("ScenarioDetailsView");

let make = (~character: CharacterApi.character, 
            ~scenario: ScenarioApi.scenario,
            ~onHpUpdate,
            ~onXpUpdate,
            ~onDrawCard,
            ~onReshuffle, 
            ~onCancel, 
            _children) => {

    ...component,

    initialState: () => { 
        character: character,  
        scenario: scenario,
        hpModalVisible: false,
        xpModalVisible: false
    },

    willReceiveProps: (self) => {...self.state, scenario: scenario},

    reducer: (action, state) => {
        let incDecValue = (incDec, value) => 
            switch (incDec) {
            | Increment => value + 1;
            | Decrement => (value <= 0) ? 0 : value - 1;
            };

        switch action {   
            | UpdateHp(incDec) => {
                let hp = incDecValue(incDec, state.scenario.health);
                let hp = (hp > state.scenario.maxHealth) ? state.scenario.maxHealth : hp;
                ReasonReact.Update({...state, scenario: {...state.scenario, health: hp}});
            }
            | UpdateXp(incDec) => {
                let xp = incDecValue(incDec, state.scenario.experience);
                ReasonReact.Update({...state, scenario: {...state.scenario, experience: xp}});
            }
            | ToggleHpModal => 
                ReasonReact.Update({...state, hpModalVisible: !state.hpModalVisible});            
            | ToggleXpModal => 
                ReasonReact.Update({...state, xpModalVisible: !state.xpModalVisible});   
        };
    },

    render: ({state, send}) => {   
        let scenario = state.scenario;  
        let deck = scenario.modifierDeck;       

        let toggleHpModal = () => send(ToggleHpModal);
        let toggleXpModal = () => send(ToggleXpModal); 

        let onDecHp = () => send(UpdateHp(Decrement));
        let onIncHp = () => send(UpdateHp(Increment));
        let onSubmitHp = () => {
            onHpUpdate(scenario.health);
            toggleHpModal();
        };

        let onDecXp = () => send(UpdateXp(Decrement));
        let onIncXp = () => send(UpdateXp(Increment));
        let onSubmitXp = () => {
            onXpUpdate(scenario.experience);
            toggleXpModal();
        };

        let onClickDrawCard = () => onDrawCard();
        let onClickReshuffle = () => onReshuffle();
        
        let cancel = () => onCancel(); 
 

        let getHpDisplay = () =>
            string_of_int(scenario.health) ++ "/" ++ string_of_int(scenario.maxHealth)
            |> Elem.string;

        let getXpDisplay = () =>
            string_of_int(scenario.experience) ++ " xp"
            |> Elem.string;

        let renderCardAction = (action, actionAmount) =>
            switch (actionAmount) {
            | Some(amount) => 
                <div>(Elem.string("Action: " ++ action ++ " " ++ string_of_int(amount)))</div>
            | None => 
                <div>(Elem.string("Action: " ++ action))</div>
            };               

        let renderCardDetails = (idx: int, card: ScenarioApi.modifierCard) => {
            <div className="box" key=("card" ++ string_of_int(idx))>
                <div>(Elem.string("Damage: " ++ string_of_int(card.damage)))</div>
                (renderCardAction(card.action, card.actionAmount))
                <div>(Elem.string("Draw Another: " ++ string_of_bool(card.drawAnother)))</div>
                <div>(Elem.string("Reshuffle: " ++ string_of_bool(card.reshuffle)))</div>
            </div>    
        };       

        let renderDiscards = (cards: list(ScenarioApi.modifierCard)) => 
            cards 
            |> List.mapi((idx, card) => renderCardDetails(idx, card))
            |> Array.of_list
            |> Elem.array;        

        let renderCurrentCard = (cardOpt: option(ScenarioApi.modifierCard)) => 
            switch (cardOpt) {
            | Some(card) => renderCardDetails(-1, card)
            | None => <div>(Elem.string("No cards have been drawn"))</div>
            };              
        
        <div className="columns">
            <div className="column is-6">
                <h3 className="title is-3">(Elem.string("Play Scenario"))</h3>

                <div>
                    <a className="button is-large is-primary is-outlined m-r-md" onClick=(Utils.handleBtnClick(toggleHpModal))>
                        <span className="icon is-small">
                            <i className="fas fa-heart"></i>
                        </span>
                        <span>(getHpDisplay())</span>
                    </a>

                    <a className="button is-large is-primary is-outlined" onClick=(Utils.handleBtnClick(toggleXpModal))>
                        <span className="icon is-small">
                            <i className="fas fa-star"></i>
                        </span>
                        <span>(getXpDisplay())</span>
                    </a>
                </div>

                <div className="columns is-mobile m-t-md">
                    <div className="column is-four-fifths">
                        <div className="is-size-4 has-text-weight-bold">(Elem.string("Current Card"))</div>
                        <div className="has-text-weight-bold">
                            (renderCurrentCard(deck.currentCard))
                        </div>
                    </div>
                </div>

                <div className="field is-grouped m-t-md">
                    <div className="control">
                        <UIButton 
                            title="Draw Card" 
                            onClick=onClickDrawCard 
                            classes="button is-primary" />
                    </div>
                    <div className="control">
                        <UIButton 
                            title="Reshuffle" 
                            onClick=onClickReshuffle 
                            classes="button is-primary" />
                    </div>
                </div>
                
                <div className="columns m-t-md is-mobile">
                    <div className="column is-four-fifths">
                        <div className="is-size-4 has-text-weight-bold">(Elem.string("Discards"))</div>
                        <div className="has-text-weight-bold">
                            (renderDiscards(deck.discards))
                        </div>
                    </div>
                </div>
        
                <div className="field is-grouped m-t-lg">
                    <div className="control">
                        <UIButton 
                            title="End Scenario" 
                            onClick=cancel 
                            classes="button is-info" />
                    </div>
                    <div className="control">
                        <UIButton 
                            title="Cancel" 
                            onClick=cancel 
                            classes="button is-light" />
                    </div>
                </div>
            </div>

            <ScenarioStatModal 
                isVisible=state.hpModalVisible
                toggleVisibility=toggleHpModal
                contentElem=(() => 
                    <ScenarioStatEditor 
                        label="Health"
                        value=state.scenario.health
                        onDecrement=onDecHp
                        onIncrement=onIncHp
                        onSubmit=onSubmitHp
                        onCancel=toggleHpModal
                        minInclusive=0
                        maxInclusive=state.scenario.maxHealth
                        showMax=true />)
            />

            <ScenarioStatModal 
                isVisible=state.xpModalVisible
                toggleVisibility=toggleXpModal
                contentElem=(() => 
                    <ScenarioStatEditor  
                        label="Experience"
                        value=state.scenario.experience
                        onDecrement=onDecXp
                        onIncrement=onIncXp
                        onSubmit=onSubmitXp
                        onCancel=toggleXpModal
                        minInclusive=0
                        showMax=false />)
            />
            
        </div>
    }
};