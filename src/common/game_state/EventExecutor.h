//
// Created by huyux on 1/5/2023.
//

#ifndef MONOPOLY_EVENT_EXECUTOR_H
#define MONOPOLY_EVENT_EXECUTOR_H

#include "events/Event.h"
// #include "../game_state/Game.h"

/**
 * This class is used to execute events.
 */

enum EventCommandType
{
    P2P_MONEY,
    P2C_MONEY,
    C2P_MONEY,
    GO_TO_ABS,
    GO_TO_REL,
    MOVE_TO_NEAREST_RAIL,
    GET_OUT_OF_JAIL_FREE_CARD, 
    GENERAL_REPAIRS,
    STREET_REPAIRS
};

struct EventCommand
{
    EventCommandType type;
    int value;
};

class EventExecutor
{
    // private:
    //     Game* _game;
public:
    EventExecutor() = default;
    ~EventExecutor() = default;

    EventCommand commandExecuteEvent(Event *event);
};

#endif // MONOPOLY_EVENT_EXECUTOR_H
