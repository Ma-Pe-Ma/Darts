#ifndef STATECONTEXT_H
#define STATECONTEXT_H

class State;

class StateContext {

protected:
    State* currentState;

public:
    void transitionTo(State* newState, bool start, bool end);
    void transitionTo(State* newState);

};

#endif