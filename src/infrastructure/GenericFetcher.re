module DataFetcher = (Config: {let name: string; type t;}) => {

    type state = 
        | Loading
        | Loaded(Config.t)
        | Error(string);
    
    let component = ReasonReact.reducerComponent("GenericFetcher-" ++ Config.name);

    let make = (
        ~fetch: (Config.t => unit, Api.apiOnError) => unit, 
        ~renderLoading=?, 
        ~renderData, 
        _children) => {

        ...component,

        initialState: () => Loading,

        reducer: (action, _state) => ReasonReact.Update(action: state),

        didMount: self => {
            let onFetchSuccess = (data) => self.send(Loaded(data));

            let onFetchError: (Api.apiOnError) = (error) => {
                switch (error) {
                | Unauthorised => Session.logout()
                | ApiError(err) => self.send(Error(err.message))
                };
            };
            
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

