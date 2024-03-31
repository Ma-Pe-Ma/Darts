#include "StateContext.h"
#include "State.h"

void StateContext::transitionTo(State* newState, bool start, bool end) {
    if (end) {
        currentState->end();
    }

    currentState = newState;
    currentState->setContext(this);

    if (start) {
        currentState->start();
    }
}

void StateContext::transitionTo(State* newState) {
    transitionTo(newState, true, true);
}