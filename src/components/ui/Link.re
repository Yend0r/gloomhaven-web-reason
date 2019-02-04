let component = ReasonReact.statelessComponent("Link");

let isModifiedEvent = (event: ReactEvent.Mouse.t) => 
        ReactEvent.Mouse.metaKey(event)
        || ReactEvent.Mouse.altKey(event)
        || ReactEvent.Mouse.ctrlKey(event)
        || ReactEvent.Mouse.shiftKey(event);

let isLinkEvent = (event: ReactEvent.Mouse.t) =>
    ReactEvent.Mouse.button(event) == 0 /* ignore everything but left clicks */
    && !isModifiedEvent(event); /* ignore clicks with modifier keys */

let make = (~text, ~path, ~className=?, _children) => {
    ...component,

    render: (_self) => {
        let handleClick = (event: ReactEvent.Mouse.t) =>
            if (isLinkEvent(event)) {
                ReactEvent.Mouse.preventDefault(event);
                ReasonReact.Router.push(path);
            };
        
        <a 
            href={path}
            className={Utils.optStringOrEmpty(className)}
            onClick={(event) => handleClick(event)}>
                (Elem.string(text))
        </a>
    }
};