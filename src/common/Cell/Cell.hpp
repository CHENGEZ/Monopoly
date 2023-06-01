#ifndef CELL
#define CELL
#include <vector>
#include "../game_state/events/Event.h"
#include "../game_state/player/Player.hpp"

using namespace std;

enum CellActivations
{
    DO_NOTHING,
    P2P_MONEY_AVENUE,
    P2P_MONEY_RAILWAY,
    P2P_MONEY_UTILITY,
    P2C_MONEY_TAX,
    // C2P_MONEY_GO,
    EVENT_TRIGGER_CHANCE,
    EVENT_TRIGGER_COMMUNITY,
    GO_TO_JAIL
};

class Cell : public serializable
{
protected:
    /**
     * @brief The string of the name of the cell
     *
     */
    const string NAME;

    /**
     * @brief A list containing the IDs of all players on the cell.
     *
     */
    vector<std::string> playerIDsOnCell;

    /**
     * @brief The ID of the cell.
     *
     */
    const int ID;


public:
    enum CellType
    {
        AVENUE,
        CHANCE,
        COMMUNITY_CHEST,
        GO_2_JAIL,
        GO_AND_FREEPARKING,
        JAIL,
        RAILWAY,
        TAX,
        UTILITY,
    };

    virtual ~Cell() = 0;
    /**
     * @brief A virtual function that will be overridden by the activate function of each different cell.
     * It serves as the callback function when a player lands on the cell.
     *
     */
    virtual CellActivations activate() = 0;

    vector<std::string> getPlayerOnCell() const;
    const string getName() const;
    const int getId() const;

    virtual const int getPrice() const;
    virtual const int getBuildingPrice() const;
    virtual const int getMortgage() const;
    virtual const int getBaseRent() const;
    virtual int getActualRent() const;
    virtual const int getMaxBuildings() const;
    virtual const int getRentPerBuilding() const;
    virtual int getCurrentNumBuildings() const;
    virtual std::string getOwnerId() const;
    virtual vector<Event *>* getEventDatabase() const;
    virtual const int getRoundToGo() const;
    virtual const int getTaxAmount() const;

    virtual void setOwnerId(std::string newOwnerID);
    virtual void setNumBuildings(int newNumBuildings);

    void playerLeave(Player *player);
    void playerArrive(Player *player);

    // serializable interface
    virtual void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const override;
    static Cell *fromJson(const rapidjson::Value &json);

protected:
    CellType cellType;
    /**
     * @brief The deserialize constructor of the cell.
     *
     *
     * @param ID The ID of the cell.
     * @param NAME The name of the cell.
     * @param playerIDsOnCell The list of IDs of all players on the cell.
     */
    Cell(const int ID, string NAME, vector<std::string> playerIDsOnCell); // base constructor

};
#endif