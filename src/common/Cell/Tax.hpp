#pragma  once
#include "Cell.hpp"

class Tax : public Cell
{
private:
    /**
     * @brief The amount of tax to pay when a player lands on it.
     *
     */
    const int TAX_AMOUNT;

public:
    Tax(const int ID, const string NAME, vector<std::string> playerIDsOnCell, const int TAX_AMOUNT);

    /**
     * @brief Call back function when a player lands on it.
     * Simply deduct TAX_AMOUNT from this player’s balance.
     *
     */
    CellActivations activate();

    const int getTaxAmount() const;

    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const;
    static Tax *fromJson(const rapidjson::Value &json);
};
