

#include "fsm.h"

static FSM* g_fsm = NULL;

FSM::FSM()
    : stat_(ST_WAIT)
{
}

FSM::~FSM()
{
}

FSM* FSM::GetInstance()
{
    if (g_fsm == NULL)
        g_fsm = new FSM();
    return g_fsm;
}


