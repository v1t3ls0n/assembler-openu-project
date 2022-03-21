#include "data.h"

static State globalState = startProgram;

void updateGlobalState(State new)
{
    globalState = new;
}

State getGlobalState()
{
    State current = globalState;
    return current;
}