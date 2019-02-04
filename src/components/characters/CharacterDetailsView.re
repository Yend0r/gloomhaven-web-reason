type state = {
    character: CharacterApi.character,
    submissionError: option(string)
};

type incDec = 
    | Increment
    | Decrement;

type action =
    | UpdateGold(incDec)
    | UpdateXp(incDec)
    | SetSubmissionError(string);

let component = ReasonReact.reducerComponent("CharacterDetailsView");

let make = (~data: CharacterApi.character, _children) => {

    ...component,

    initialState: () => {
        character: data,
        submissionError: None
    },

    reducer: (action, state) => {
        let ch = state.character;

        let incDecValue = (incDec, value) => 
            switch (incDec) {
            | Increment => value + 1;
            | Decrement => (value <= 0) ? 0 : value - 1;
            };

        switch action {
            | UpdateGold(incDec) => {
                let gold = incDecValue(incDec, ch.gold);
                ReasonReact.Update({...state, character: {...ch, gold: gold}});
            }
            | UpdateXp(incDec) => {
                let xp = incDecValue(incDec, ch.experience);
                ReasonReact.Update({...state, character: {...ch, experience: xp}});
            }
            | SetSubmissionError(errorMsg) => 
                ReasonReact.Update({...state, submissionError: Some(errorMsg)})
        };
    },

    render: ({send, state}) => {
        let ch = state.character;

        let onDecGold = () => send(UpdateGold(Decrement));
        let onIncGold = () => send(UpdateGold(Increment));

        let onDecXp = () => send(UpdateXp(Decrement));
        let onIncXp = () => send(UpdateXp(Increment));

        <div className="columns">
            <div className="column">
                <div className="content is-medium">

                    <div className="box">

                        <div className="level">
                            <div className="level-left">
                                <div className="level-item">
                                    <p className="is-size-3 is-5">
                                        <strong>(Elem.string("Character: " ++ ch.name))</strong>
                                    </p>
                                </div>
                            </div>

                            <div className="level-right">
                                <div className="level-item">
                                    <p className="is-size-3 is-5">
                                        <strong>(Elem.string("Class: " ++ ch.ghClassName))</strong>
                                    </p>
                                </div>
                            </div>
                        </div>

                        <article className="message is-primary">
                            <div className="message-body">

                                <CharDetailEditor 
                                    label="Gold Earned"
                                    value=ch.gold
                                    onDecrement=onDecGold
                                    onIncrement=onIncGold
                                    layout=Horizontal />
                                
                                <CharDetailEditor 
                                    label="Experience"
                                    value=ch.experience
                                    onDecrement=onDecXp
                                    onIncrement=onIncXp 
                                    layout=Horizontal />
                                
                            </div>
                        </article>
                    </div>

                </div>
            </div>
        </div>
    }
};