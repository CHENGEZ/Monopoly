//
// Created by huyux on 1/5/2023.
//

#include "EventExecutor.h"

EventCommand EventExecutor::commandExecuteEvent(Event *event)
{
    // return EventCommand();
    switch (event->getType())
    {
        case Event::ADVANCE_TO_GO:
            return {GO_TO_ABS, 0};
        case Event::BANK_ERROR_IN_YOUR_FAVOR:
            return {C2P_MONEY, 75};
        case Event::DOCTOR_FEE:
            return {P2C_MONEY, 50};
        case Event::SALE_OF_STOCK:
            return {P2C_MONEY, 50};
        case Event::GO_TO_JAIL:
            return {GO_TO_ABS, 10};
        case Event::HOLIDAY_FUND_MATURES:
            return {C2P_MONEY, 100};
        case Event::INCOME_TAX_REFUND:
            return {C2P_MONEY, 20};
        case Event::IT_IS_YOUR_BIRTHDAY:
            return {P2P_MONEY, -10};
        case Event::LIFE_INSURANCE_MATURES:
            return {C2P_MONEY, 100};
        case Event::PAY_HOSPITAL_FEES:
            return {P2C_MONEY, 100};
        case Event::PAY_SCHOOL_FEES:
            return {P2C_MONEY, 150};
        case Event::CONSULTANCY_FEE:
            return {C2P_MONEY, 25};
        case Event::STREET_REPAIRS:
            return {STREET_REPAIRS, 40};
        case Event::SECOND_PRIZE_IN_BEAUTY_CONTEST:
            return {C2P_MONEY, 10};
        case Event::INHERIT_100:
            return {C2P_MONEY, 100};
        case Event::ADVANCE_TO_BROADWALK:
            return {GO_TO_ABS, 39};
        case Event::ADVANCE_TO_ILLINOIS_AVE:
            return {GO_TO_ABS, 24};
        case Event::ADVANCE_TO_ST_CHARLES_PLACE:
            return {GO_TO_ABS, 11};
        case Event::ADVANCE_TO_NEAREST_RAILROAD:
            return {MOVE_TO_NEAREST_RAIL, 0};
        case Event::BANK_PAYS_DIVIDEND:
            return {C2P_MONEY, 50};
        case Event::GO_BACK_THREE_SPACES:
            return {GO_TO_REL, -3};
        case Event::MAKE_GENERAL_REPAIRS:
            return {GENERAL_REPAIRS, 25};
        case Event::SPEEDING_FINE:
            return {P2C_MONEY, 15};
        case Event::ADVANCE_TO_READING_RAILROAD:
            return {GO_TO_ABS, 5};
        case Event::CHAIRMAN_OF_THE_BOARD:
            return {P2P_MONEY, 50};
        case Event::BUILDING_LOAN_MATURES:
            return {C2P_MONEY, 150};
        case Event::GET_OUT_OF_JAIL_FREE:
            return {GET_OUT_OF_JAIL_FREE_CARD, 0};
        default:
            return {P2C_MONEY, 0};
    }
}

