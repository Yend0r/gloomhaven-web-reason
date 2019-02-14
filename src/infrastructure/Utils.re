
/* This is a bit lame, but it'll do for now */
let stringIsEmail = (str) => 
    String.contains(str, '@') && String.contains(str, '.') && String.length(str) > 3;

let stringIsEmpty = (str) => {
    let trimmedStr = String.trim(str);
    let strLength = String.length(trimmedStr);
    (strLength == 0);
};

let combineStringOptions = (str1: option(string), str2: option(string)) =>
    switch (str1, str2) {
        | (Some(s), _) | (_, Some(s)) => Some(s)
        | (_, _) => None
    };

let optStringOrEmpty = (optStr: option(string)) => 
    switch (optStr) {
        | Some(s) => s
        | None => ""
    };

let listTryFind = (predicate: 'a => bool, l: list('a)) =>
    switch(List.exists(predicate, l)) {
    | true => Some(List.find(predicate, l))
    | false => None
    };

let valueFromEvent = (evt): string => (
    evt |> ReactEvent.Form.target
)##value;

let handleBtnClick = (fn) => (event) => {
    ReactEvent.Mouse.preventDefault(event);
    fn();
};