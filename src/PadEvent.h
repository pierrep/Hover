#pragma once
#include "ofMain.h"


class PadEventArgs : public ofEventArgs {

public:

    int     index;
    bool    state;

    PadEventArgs() {
        index = -1;
        state = false;
    }

    static ofEvent <PadEventArgs> padEvent;
};


