#pragma  once
#include "Cell.hpp"
#include <string>
using namespace std;

class Avenue : public Cell
{
private:
    /**
     * @brief The price of the avenue, i.e., the amount that the player must pay when buying it.
     *
     */
    const int PRICE;

    /**
     * @brief The base rent that players other than the owner must pay when landing on it.
     *
     */
    const int BASE_RENT;

    /**
     * @brief The maximum number of buildings that is allowed to be built on the avenue.
     *
     */
    const int MAX_BUILDINGS;

    /**
     * @brief The amount of rent that will be increased by the existence of each building.
     *
     */
    const int RENT_PER_BUILDING;

    /**
     * @brief The price that each player needs to pay when adding a building to this property.
     *
     */
    const int BUILDING_PRICE;

    /**
     * @brief The price that the player will recieve when selling this property.
     *
     */
    const int MORTGAGE;

    /**
     * @brief The current number of buildings built on this avenue.
     * The actual rent of this avenue is BASE_RENT + numBuilding * RENT_PER_BUILDING.
     *
     */
    int numBuildings;

    /**
     * @brief The ID of the owner of this property.
     * Mainly for finding to whom the rent should be paid.
     *
     */
    std::string ownerID;



public:
    /*
     * Public constructor for creation and deserialization
     */
    Avenue(int ID, const string NAME, vector<std::string> playerIDsOnCell, const int PRICE, const int BASE_RENT, const int MAX_BUILDINGS, const int RENT_PER_BUILDING, const int BUILDING_PRICE, const int MORTGAGE, std::string ownerID, int numBuildings);

    /**
     * @brief Call back function when a player lands on it.
     * Depending on whether the property has an owner it will have different behaviors.
     *
     */
    CellActivations activate();

    const int getPrice() const;
    const int getBuildingPrice() const;
    const int getMortgage() const;
    const int getBaseRent() const;
    const int getMaxBuildings() const;
    const int getRentPerBuilding() const;
    int getCurrentNumBuildings() const;
    std::string getOwnerId() const;

    int getActualRent() const;

    void setOwnerId(std::string newOwnerID);
    void setNumBuildings(int newNumBuildings);

    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const;
    static Avenue *fromJson(const rapidjson::Value &json);
};
