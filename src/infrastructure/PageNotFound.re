let component = ReasonReact.statelessComponent("PageNotFound");

let make = (_children) => {

    ...component,

    render: _self =>{
        let goHome = () => Routes.navigate(Routes.Home);

        <div className="container has-text-centered">
            <div className="column is-8 is-offset-2">
                <div className="box p-t-xl p-b-xl">
                    <h3 className="title has-text-grey">
                        (Elem.string("404 Page Not Found"))
                    </h3>    
                    <p className="subtitle has-text-grey">(Elem.string("Please return to the home page and try again."))</p>                    
                    <button 
                        className="button is-primary is-large"
                        onClick=(Utils.handleBtnClick(goHome))>
                        (Elem.string("Home"))
                    </button>
                </div>
            </div>
        </div>
    }
};