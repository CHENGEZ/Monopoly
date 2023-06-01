#include "Event.h"

Event::~Event() {}

std::string Event::getDescription() const noexcept
{
    return _description;
}

Event::Type Event::getType() const noexcept
{
    return _type;
}

void Event::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
{
    // _description
    rapidjson::Value descriptionValue(_description.c_str(), allocator);
    json.AddMember("description", descriptionValue, allocator);

    // _type
    rapidjson::Value typeValue(type2String(_type).c_str(), allocator);
    json.AddMember("type", typeValue, allocator);
}

Event *Event::fromJson(const rapidjson::Value &json)
{
    // std::string _description = json["description"].GetString();
    Event::Type _type = Event::string2Type(json["type"].GetString());

    return new Event(_type);
}


std::string Event::type2String(Type type) const
{
    switch (type)
    {
    case ADVANCE_TO_GO:
        return "Advance to Go (Collect $200)";
    case BANK_ERROR_IN_YOUR_FAVOR:
        return "Bank error in your favor - Collect $75";
    case DOCTOR_FEE:
        return "Doctor's fees - Pay $50";
    case SALE_OF_STOCK:
        return "From sale of stock you get $50";
    case GET_OUT_OF_JAIL_FREE:
        return "Get Out of Jail Free - This card may be kept until needed.";
    case GO_TO_JAIL:
        return "Go to Jail - Go directly to jail - Do not pass Go, do not collect $200";
    case HOLIDAY_FUND_MATURES:
        return "Holiday Fund matures - Receive $100";
    case INCOME_TAX_REFUND:
        return "Income tax refund - Collect $20";
    case IT_IS_YOUR_BIRTHDAY:
        return "It is your birthday Collect $10 from each player";
    case LIFE_INSURANCE_MATURES:
        return "Life insurance matures - Collect $100";
    case PAY_HOSPITAL_FEES:
        return "Pay hospital fees of $100";
    case PAY_SCHOOL_FEES:
        return "Pay school fees of $150";
    case CONSULTANCY_FEE:
        return "Receive $25 consultancy fee";
    case STREET_REPAIRS:
        return "You are assessed for street repairs - $40 per building";
    case SECOND_PRIZE_IN_BEAUTY_CONTEST:
        return "You have won second prize in a beauty contest - Collect $10";
    case INHERIT_100:
        return "You inherit $100";
    case ADVANCE_TO_BROADWALK:
        return "Advance to Boardwalk";
    case ADVANCE_TO_ILLINOIS_AVE:
        return "Advance to Illinois Ave";
    case ADVANCE_TO_ST_CHARLES_PLACE:
        return "Advance to St. Charles Place";
    case ADVANCE_TO_NEAREST_RAILROAD:
        return "Advance token to nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.";
    case BANK_PAYS_DIVIDEND:
        return "Bank pays you dividend of $50";
    case GO_BACK_THREE_SPACES:
        return "Go Back 3 Spaces";
    case MAKE_GENERAL_REPAIRS:
        return "Make general repairs on all your property - For each building pay $25";
    case SPEEDING_FINE:
        return "Speeding fine $15";
    case ADVANCE_TO_READING_RAILROAD:
        return "Take a trip to Reading Railroad - If you pass Go, collect $200";
    case CHAIRMAN_OF_THE_BOARD:
        return "You have been elected Chairman of the Board - Pay each player $50";
    case BUILDING_LOAN_MATURES:
        return "Your building loan matures - Collect $150";
    default:
        return "";
    }
}

Event::Type Event::string2Type(std::string str)
{
    if (str == "Advance to Go (Collect $200)")
        return ADVANCE_TO_GO;
    else if (str == "Bank error in your favor - Collect $75")
        return BANK_ERROR_IN_YOUR_FAVOR;
    else if (str == "Doctor's fees - Pay $50")
        return DOCTOR_FEE;
    else if (str == "From sale of stock you get $50")
        return SALE_OF_STOCK;
    else if (str == "Get Out of Jail Free - This card may be kept until needed.")
        return GET_OUT_OF_JAIL_FREE;
    else if (str == "Go to Jail - Go directly to jail - Do not pass Go, do not collect $200")
        return GO_TO_JAIL;
    else if (str == "Holiday Fund matures - Receive $100")
        return HOLIDAY_FUND_MATURES;
    else if (str == "Income tax refund - Collect $20")
        return INCOME_TAX_REFUND;
    else if (str == "It is your birthday Collect $10 from each player")
        return IT_IS_YOUR_BIRTHDAY;
    else if (str == "Life insurance matures - Collect $100")
        return LIFE_INSURANCE_MATURES;
    else if (str == "Pay hospital fees of $100")
        return PAY_HOSPITAL_FEES;
    else if (str == "Pay school fees of $150")
        return PAY_SCHOOL_FEES;
    else if (str == "Receive $25 consultancy fee")
        return CONSULTANCY_FEE;
    else if (str == "You are assessed for street repairs - $40 per building")
        return STREET_REPAIRS;
    else if (str == "You have won second prize in a beauty contest - Collect $10")
        return SECOND_PRIZE_IN_BEAUTY_CONTEST;
    else if (str == "You inherit $100")
        return INHERIT_100;
    else if (str == "Advance to Boardwalk")
        return ADVANCE_TO_BROADWALK;
    else if (str == "Advance to Illinois Ave")
        return ADVANCE_TO_ILLINOIS_AVE;
    else if (str == "Advance to St. Charles Place")
        return ADVANCE_TO_ST_CHARLES_PLACE;
    else if (str == "Advance token to nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.")
        return ADVANCE_TO_NEAREST_RAILROAD;
    else if (str == "Bank pays you dividend of $50")
        return BANK_PAYS_DIVIDEND;
    else if (str == "Go Back 3 Spaces")
        return GO_BACK_THREE_SPACES;
    else if (str == "Make general repairs on all your property - For each building pay $25")
        return MAKE_GENERAL_REPAIRS;
    else if (str == "Speeding fine $15")
        return SPEEDING_FINE;
    else if (str == "Take a trip to Reading Railroad - If you pass Go, collect $200")
        return ADVANCE_TO_READING_RAILROAD;
    else if (str == "You have been elected Chairman of the Board - Pay each player $50")
        return CHAIRMAN_OF_THE_BOARD;
    else if (str == "Your building loan matures - Collect $150")
        return BUILDING_LOAN_MATURES;
    else
        return Type();
}

Event::Event(Type type) : _type(type)
{
    switch (type)
    {
    case ADVANCE_TO_GO:
        _description = "Advance to Go (Collect $200)";
        break;
    case BANK_ERROR_IN_YOUR_FAVOR:
        _description = "Bank error in your favor - Collect $75";
        break;
    case DOCTOR_FEE:
        _description = "Doctor's fees - Pay $50";
        break;
    case SALE_OF_STOCK:
        _description = "From sale of stock you get $50";
        break;
    case GET_OUT_OF_JAIL_FREE:
        _description = "Get Out of Jail Free - This card may be kept until needed.";
        break;
    case GO_TO_JAIL:
        _description = "Go to Jail - Go directly to jail - Do not pass Go, do not collect $200";
        break;
    case HOLIDAY_FUND_MATURES:
        _description = "Holiday Fund matures - Receive $100";
        break;
    case INCOME_TAX_REFUND:
        _description = "Income tax refund - Collect $20";
        break;
    case IT_IS_YOUR_BIRTHDAY:
        _description = "It is your birthday Collect $10 from each player";
        break;
    case LIFE_INSURANCE_MATURES:
        _description = "Life insurance matures - Collect $100";
        break;
    case PAY_HOSPITAL_FEES:
        _description = "Pay hospital fees of $100";
        break;
    case PAY_SCHOOL_FEES:
        _description = "Pay school fees of $150";
        break;
    case CONSULTANCY_FEE:
        _description = "Receive $25 consultancy fee";
        break;
    case STREET_REPAIRS:
        _description = "You are assessed for street repairs - $40 per building";
        break;
    case SECOND_PRIZE_IN_BEAUTY_CONTEST:
        _description = "You have won second prize in a beauty contest - Collect $10";
        break;
    case INHERIT_100:
        _description = "You inherit $100";
        break;
    case ADVANCE_TO_BROADWALK:
        _description = "Advance to Boardwalk";
        break;
    case ADVANCE_TO_ILLINOIS_AVE:
        _description = "Advance to Illinois Ave";
        break;
    case ADVANCE_TO_ST_CHARLES_PLACE:
        _description = "Advance to St. Charles Place";
        break;
    case ADVANCE_TO_NEAREST_RAILROAD:
        _description = "Advance token to nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank.";
        break;
    case BANK_PAYS_DIVIDEND:
        _description = "Bank pays you dividend of $50";
        break;
    case GO_BACK_THREE_SPACES:
        _description = "Go Back 3 Spaces";
        break;
    case MAKE_GENERAL_REPAIRS:
        _description = "Make general repairs on all your property - For each building pay $25";
        break;
    case SPEEDING_FINE:
        _description = "Speeding fine $15";
        break;
    case ADVANCE_TO_READING_RAILROAD:
        _description = "Take a trip to Reading Railroad - If you pass Go, collect $200";
        break;
    case CHAIRMAN_OF_THE_BOARD:
        _description = "You have been elected Chairman of the Board - Pay each player $50";
        break;
    case BUILDING_LOAN_MATURES:
        _description = "Your building loan matures - Collect $150";
        break;
    default:
        break;
    }
}

// TODO: deserialization constructor