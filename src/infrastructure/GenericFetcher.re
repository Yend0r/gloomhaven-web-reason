module DataFetcher = (Config: {let name: string; type t;}) => {

    type state = 
        | Loading
        | Loaded(Config.t)
        | Error(string);
    
    let component = ReasonReact.reducerComponent("GenericFetcher-" ++ Config.name);

    let make = (
        ~fetch: (Config.t => unit, string => unit) => unit, 
        ~renderLoading=?, 
        ~renderData, 
        _children) => {

        ...component,

        initialState: () => Loading,

        reducer: (action, _state) => ReasonReact.Update(action: state),

        didMount: self => {
            let onFetchSuccess = (data) => self.send(Loaded(data));
            let onFetchError = (error) => self.send(Error(error));
            
            fetch(onFetchSuccess, onFetchError);
        },

        render: self => {

            let onLoading = () =>
                switch (renderLoading) {
                | Some(fn) => fn()
                | None => <div>(Elem.string("loading..."))</div>
                }

            switch (self.state) {
                | Loaded(data) => 
                    renderData(data)
                | Loading => 
                    onLoading()
                | Error(errorMsg) => 
                    <GenericError error={Some(errorMsg)} />
            };
        }
    };
};

