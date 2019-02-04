module CharacterDetailsFetcher = GenericFetcher.DataFetcher({
    let name = "CharacterDetails";
    type t = CharacterApi.character;
});

let make = (~characterId, children) => { 
    let fetch = (onSuccess, onError) => CharacterApi.get(characterId, onSuccess, onError); 
    CharacterDetailsFetcher.make(
        ~fetch, 
        ~renderData = ((data) => <CharacterDetailsView data={data} />),
        children
    ) 
};