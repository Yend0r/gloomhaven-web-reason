module GhClassListFetcher = GenericFetcher.DataFetcher({
    let name = "GhClassChooser";
    type t = list(GameDataApi.ghClass);
});

let make = (~selected, ~onChange, ~hasError, children) => { 

    let fetch = (onSuccess, onError) => GameDataApi.getList(onSuccess, onError); 

    let getTitle = (item: GameDataApi.ghClass) =>
        switch (item.isStarting) {
        | true => item.ghClassName ++ " (starting class)"
        | false => item.ghClassName
        };

    let renderOption = (idx: int, item: GameDataApi.ghClass) => 
        <option 
            key={"ghClassOption" ++ string_of_int(idx)} 
            value=(item.ghClassName)>(Elem.string(getTitle(item)))</option>;

    let renderOptions = (data: list(GameDataApi.ghClass)) => 
        List.mapi((idx, item) => renderOption(idx, item), data)
        |> Array.of_list
        |> ReasonReact.array;

    let cssClasses = hasError ? "select is-danger" : "select";

    let renderSelect = (data: list(GameDataApi.ghClass)) => 
        <div className=cssClasses>
            <select 
                id="ghClassName"
                name="ghClassName"
                onChange=onChange
                defaultValue=selected>

                <option value=""></option>
                (renderOptions(data))

            </select>
        </div>;

    let renderLoading = () =>
        <div className="control">
            <div className="select is-loading">
                <select>
                    <option>(Elem.string("Classes..."))</option>
                </select>
            </div>
        </div>;

    GhClassListFetcher.make(
        ~fetch, 
        ~renderLoading=renderLoading,
        ~renderData=renderSelect,
        children
    ) 
};