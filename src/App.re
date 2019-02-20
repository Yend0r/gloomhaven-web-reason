
type state = {
    currentUrl: ReasonReact.Router.url
};

type action = Navigate(ReasonReact.Router.url);

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
    ...component,

    initialState: () => {
        currentUrl: ReasonReact.Router.dangerouslyGetInitialUrl()
    },

    didMount: self => {
        let watcherID = ReasonReact.Router.watchUrl(url => Navigate(url) |> self.send);
        self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
    },

    reducer: (action, _state) => {
        switch(action) {
            | Navigate(url) => ReasonReact.Update({currentUrl: url})
        };
    },

    render: ({state}) => {
        let renderContent = () => {
            let isLoggedIn = Session.isLoggedIn();
            let routePage = Router.getPageForRoute(isLoggedIn, state.currentUrl);
            let page = routePage.renderPage();

            switch (routePage.layout) {
                | DefaultLayout => 
                    <DefaultLayout page /> 
                | LoginLayout => 
                    <LoginLayout page />
                };
        };

        {renderContent()}  
    }
};