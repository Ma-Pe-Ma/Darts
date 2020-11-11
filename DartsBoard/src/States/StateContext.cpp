#include "StateContext.h"
#include "State.h"

void StateContext::TransitionTo(State* newState, bool start, bool end) {
    if (end) {
        currentState->End();
    }

    currentState = newState;
    currentState->SetContext(this);

    if (start) {
        currentState->Start();
    }
}

void StateContext::TransitionTo(State* newState) {
    TransitionTo(newState, true, true);
}