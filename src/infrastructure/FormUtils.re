
type fieldError = {
    fieldName: string,
    error: string
};

let addError = (fieldName, error, fieldErrors: list(fieldError)) => 
    [{fieldName: fieldName, error: error}, ...fieldErrors];

let removeError = (fieldName, fieldErrors: list(fieldError)) => 
    List.filter(e => e.fieldName == fieldName, fieldErrors);

let tryFindError = (fieldName, fieldErrors: list(fieldError)) => 
    Utils.listTryFind(e => e.fieldName == fieldName, fieldErrors);

let renderError = (fError: option(fieldError)) => {
    switch(fError) {
    | Some(e) => {
        <div className="invalid-message">
            (Elem.string(e.error))
        </div>
    }
    | None => Elem.nullEl
    };
};

let validateRequiredString = (value, fieldName, error, fieldErrors) => {
    let isEmpty: bool = Utils.stringIsEmpty(value);
    switch(isEmpty) {
    | true => addError(fieldName, error, fieldErrors)
    | false => fieldErrors
    };
};

let validateRequiredEmail = (value, fieldName, error, fieldErrors) => {
    let isEmail: bool = Utils.stringIsEmail(value);
    switch(isEmail) {
    | false => addError(fieldName, error, fieldErrors)
    | true => fieldErrors
    };
};

let validateRequiredInt = (value, fieldName, error, fieldErrors) => {
    let optValue: option(int) = Convert.toIntOpt(value);
    switch(optValue) {
    | None => addError(fieldName, error, fieldErrors)
    | Some(_) => fieldErrors
    };
};

let validateRequiredPositiveInt = (value, fieldName, error, fieldErrors) => {
    let optValue: option(int) = Convert.toIntOpt(value);
    switch(optValue) {
    | None => addError(fieldName, error, fieldErrors)
    | Some(x) when x < 0 => addError(fieldName, error, fieldErrors)
    | Some(_) => fieldErrors
    };
};

let validateOptionalInt = (value, fieldName, error, fieldErrors) => {
    let optValue: option(int) = Convert.toIntOpt(value);
    let isEmpty: bool = Utils.stringIsEmpty(value);
    switch(isEmpty, optValue) {
    | (false, None) => addError(fieldName, error, fieldErrors)
    | (true, None) => fieldErrors
    | (_, Some(_)) => fieldErrors
    };
};

let validateRequiredId = (idValue, fieldName, error, fieldErrors) => {
    if (idValue <= 0) {
        addError(fieldName, error, fieldErrors)
    } else {
        fieldErrors
    };
};