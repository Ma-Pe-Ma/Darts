#ifndef STATE_H
#define STATE_H

#include "../Hardware/Pair.h"

class StateContext;

class State {
protected:
    StateContext* stateContext;

public:
    virtual void start() = 0;
    virtual void update(Pair) = 0;
    virtual void end() {}

    void setContext(StateContext*);

    virtual void initialize() {}
};

#endif