#pragma  once
#include "Cell.hpp"

class Railway : public Cell
{
private:
    /**
     * @brief The price of the railway, i.e., the amount that the player must pay when buying it
     *
     */
    const int PRICE;

    /**
     * @brief The price that the player will recieve when selling this property.
     *
     */
    const int MORTGAGE;

    /**
     * @brief The rent of one railway.
     * The actual rent of a railway depends on how many railways the owner of this railway owns. The total rent = BASE_RENT times the number of railways the owner of this railway owns.
     * The total rent = BASE_RENT times the number of railways the owner of this railway owns.
     *
     */
    const int BASE_RENT;

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
    Railway(const int ID, const std::string NAME, vector<std::string> playerIDsOnCell, const int PRICE, const int MORTGAGE, const int BASE_RENT, std::string ownerID);


    /**
     * @brief Call back function when a player lands on it.
     * Depending on whether the property has an owner it will have different behaviors.
     *
     */
    CellActivations activate();

    const int getPrice() const;
    const int getMortgage() const;
    const int getBaseRent() const;
    std::string getOwnerId() const;

    void setOwnerId(std::string newOwnerID);

    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const;
    static Railway *fromJson(const rapidjson::Value &json);
};
