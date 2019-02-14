
/* Set to false to stop logging */
let isLogging = true;

let log = (msg) => {
    if (isLogging) {
        Js.log(msg);
    };
};