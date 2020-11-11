#ifndef STATECONTEXT_H
#define STATECONTEXT_H

class State;

class StateContext {

protected:
    State* currentState;

public:
    void TransitionTo(State* newState, bool start, bool end);
    void TransitionTo(State* newState);

};

#endif