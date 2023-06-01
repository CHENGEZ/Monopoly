//
// Created by Manuel on 25.01.2021.
//

#ifndef MONOPOLY_EVENT_H
#define MONOPOLY_EVENT_H

#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/serializable.h"

/**
 * Represents an event.
 */
class Event : public serializable
{ // Inspired by card.h

public:
    enum Type
    {
        // Refer to https://www.monopolyland.com/list-monopoly-chance-community-chest-cards/
        // Community Chest
        ADVANCE_TO_GO,
        BANK_ERROR_IN_YOUR_FAVOR,
        DOCTOR_FEE,
        SALE_OF_STOCK,
        GET_OUT_OF_JAIL_FREE,
        GO_TO_JAIL,
        HOLIDAY_FUND_MATURES,
        INCOME_TAX_REFUND,
        IT_IS_YOUR_BIRTHDAY,
        LIFE_INSURANCE_MATURES,
        PAY_HOSPITAL_FEES,
        PAY_SCHOOL_FEES,
        CONSULTANCY_FEE,
        STREET_REPAIRS,
        SECOND_PRIZE_IN_BEAUTY_CONTEST,
        INHERIT_100,

        // Chance
        ADVANCE_TO_BROADWALK,
        ADVANCE_TO_ILLINOIS_AVE,
        ADVANCE_TO_ST_CHARLES_PLACE,
        ADVANCE_TO_NEAREST_RAILROAD, // Contains 2 cards
        BANK_PAYS_DIVIDEND,
        GO_BACK_THREE_SPACES,
        MAKE_GENERAL_REPAIRS,
        SPEEDING_FINE,
        ADVANCE_TO_READING_RAILROAD,
        CHAIRMAN_OF_THE_BOARD,
        BUILDING_LOAN_MATURES

    };

    Event(Type type);

    ~Event();

    // accessors
    std::string getDescription() const noexcept;
    Type getType() const noexcept;

    // serializable interface
    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const override;
    static Event *fromJson(const rapidjson::Value &json);

private:
    Type _type;
    std::string _description;
    std::string type2String(Type type) const;
    static Type string2Type(std::string str);
};

#endif // MONOPOLY_EVENT_H
