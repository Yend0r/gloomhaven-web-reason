let component = ReasonReact.statelessComponent("ScenarioStatModal");

let make = (~isVisible, ~toggleVisibility, ~contentElem, _children) => {

  ...component,

  render: (_self) => {
            
        let modalCss = 
                switch (isVisible) {
                | true => "modal is-active"
                | false => "modal"
                };

        <div className=modalCss>
            <div className="modal-background"></div>
            <div className="modal-content modal-content-half">
                
                <div className="box">
                    <div className="columns">
                        <div className="column is-half is-offset-one-quarter">
                            <div className="content">
                                (contentElem())                                                                   
                            </div>                               
                        </div>
                    </div>
                </div>
            </div>
            <button className="modal-close is-large" ariaLabel="close" onClick=(Utils.handleBtnClick(toggleVisibility))></button>
        </div>
    }
};