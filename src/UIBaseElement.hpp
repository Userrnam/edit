#pragma once


// Element - actual element which will be displayed
// Container - contains Elements and other Containers

enum class UIElementType {
    ELEMENT,
    CONTAINER
};

struct UIBaseElement {
    UIElementType elementType;

    // move it somewhere else
    float padding = 5;

    UIBaseElement(UIElementType t) {
        elementType = t;
    }
};
