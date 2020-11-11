#ifndef STATE_H
#define STATE_H

#include "../Hardware/Pair.h"

class StateContext;

class State {
protected:
    StateContext* stateContext;

public:
    virtual void Start() {}
    virtual void Update(Pair) = 0;
    virtual void End() {}

    void SetContext(StateContext*);
};

#endif