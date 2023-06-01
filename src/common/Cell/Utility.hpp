#pragma  once
#include "Cell.hpp"

class Utility : public Cell
{
private:
    /**
     * @brief The price of the Utility, i.e., the amount that the player must pay when buying it
     *
     */
    const int PRICE;

    /**
     * @brief The price that the player will recieve when selling this property.
     *
     */
    const int MORTGAGE;

    /**
     * @brief The ID of the owner of this building.
     * Mainly for finding to whom the rent should be paid and also for determining the actual rent.
     *
     */
    std::string ownerID;

public:
    /*
     * Public constructor for deserialization and creation
     */
    Utility(const int ID, const std::string NAME, vector<std::string> playerIDsOnCell, 
            const int PRICE, const int MORTGAGE, std::string ownerID);


    /**
     * @brief Call back function when a player lands on it.
     * Depending on whether the property has an owner it will have different behaviors.
     *
     */
    CellActivations activate();

    const int getPrice() const;
    const int getMortgage() const;
    std::string getOwnerId() const;

    void setOwnerId(std::string newOwnerID);

    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const;
    static Utility *fromJson(const rapidjson::Value &json);
};
