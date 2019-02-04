type navbarState = {
    isMobileMenuVisible: bool
};

type action =
    | ToggleMenu(bool);

let component = ReasonReact.reducerComponent("Navbar");

let make = (_children) => {
    ...component,

    initialState: () => {
        isMobileMenuVisible: false
    },

    reducer: (action, _state) => {
        switch action {
        | ToggleMenu(menuVisiblity) => ReasonReact.Update({isMobileMenuVisible: menuVisiblity})
        }
    },

    render: (self) => {   
        let getName = () => 
            switch (Session.getCurrentUser()) {
            | Some(u) => u.name 
            | None => "Unknown"
            };

        let logout = (_event) => {
            Session.logout();
            Routes.navigate(Auth(Routes.AuthRoutes.Login));
        };

        let gotoCharacters = () => 
            Routes.navigate(Routes.Character(List));

        let gotoProfile = () => 
            Routes.navigate(Routes.Character(List));

        let toggleMenu = (_event) => 
            self.send(ToggleMenu(!self.state.isMobileMenuVisible));
        
        let appendIsActive = (currentClass) => 
            if (self.state.isMobileMenuVisible) {
                currentClass ++ " is-active"
            } else {
                currentClass
            };        

        let renderName = () => 
            if (self.state.isMobileMenuVisible) {
                Elem.nullEl
            } else {
                Elem.string(getName())
            };

        <nav className="navbar is-primary">
            <div className="navbar-brand">
                <div className="navbar-item brand-text is-size-5">
                    (Elem.string("GloomChars"))
                </div>
                <a role="button" 
                    className=(appendIsActive("navbar-burger")) 
                    ariaLabel="menu" 
                    ariaExpanded=false
                    onClick=(Utils.handleBtnClick(toggleMenu))>
                    <span ariaHidden=true></span>
                    <span ariaHidden=true></span>
                    <span ariaHidden=true></span>
                </a>
            </div>
            
            <div className=(appendIsActive("navbar-menu")) id="navbarMenu">
                <div className="navbar-start">
                    <a className="navbar-item is-size-5" onClick=(Utils.handleBtnClick(gotoCharacters))>
                        (Elem.string("Characters"))
                    </a>
                    <a className="navbar-item is-size-5" onClick=(Utils.handleBtnClick(gotoProfile))>
                        (Elem.string("Profile"))
                    </a>
                </div>
                <div className="navbar-end">
                    <div className="navbar-item  is-size-5">(renderName())</div>
                    <div className="navbar-item">
                        <div className="buttons">
                            <button className="button is-light" onClick=logout>
                                (Elem.string("Log out"))
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        </nav>
    }
};

