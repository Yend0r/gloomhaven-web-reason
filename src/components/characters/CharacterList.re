
module CharacterListFetcher = GenericFetcher.DataFetcher({
    let name = "CharacterList";
    type t = list(CharacterApi.characterListItem);
});

let make = (children) => { 
    let fetch = (onSuccess, onError) => CharacterApi.getList(onSuccess, onError); 
    CharacterListFetcher.make(
        ~fetch, 
        ~renderData = ((data) => <CharacterListView data={data} />),
        children
    ) 
};