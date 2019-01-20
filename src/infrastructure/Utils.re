/* This is a bit lame, but it'l do for now */
let stringIsEmail = (str) => 
    String.contains(str, '@') && String.contains(str, '.') && String.length(str) > 3;

let stringIsEmpty = (str) => {
    let trimmedStr = String.trim(str);
    let strLength = String.length(trimmedStr);
    (strLength == 0);
};