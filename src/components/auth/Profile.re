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

let component = ReasonReact.statelessComponent("Profile");

let make = (_children) => {

    ...component,

    render: _ => {

        let renderProfile = (user: Session.sessionUser) => {
            <div className="columns">
                <div className="column">
                    <div className="content is-medium">

                        <div className="box">

                            <div className="level">
                                <div className="level-left">
                                    <div className="level-item">
                                        <p className="is-size-3 is-5">
                                            <strong>(Elem.string("Name: " ++ user.name))</strong>
                                        </p>
                                    </div>
                                </div>

                                <div className="level-right">
                                    <div className="level-item">
                                        <p className="is-size-3 is-5">
                                            <strong>(Elem.string("Email: " ++ user.email))</strong>
                                        </p>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        };

        switch (Session.getCurrentUser()) {
            | Some(user) => renderProfile(user)
            | None => <div>(Elem.string("User not found... wtf?"))</div>
        };
    }
};