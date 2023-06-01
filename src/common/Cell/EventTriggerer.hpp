#pragma  once
#include "Cell.hpp"

class CommunityChest : public Cell
{

public:
    /*
     * Public constructor for deserialization and creation
     */
    CommunityChest(const int ID, const string NAME, vector<std::string> playerIDsOnCell);

    /**
     * @brief Call back function when a player lands on a community chest cell.
     * It will randomly draw one event from the event database and call the call() function from the Event class.
     *
     */
    CellActivations activate();
};

class Chance : public Cell
{

public:
    /*
     * Public constructor for deserialization and creation
     */
    Chance(const int ID, const string NAME, vector<std::string> playerIDsOnCell);
    /**
     * @brief Call back function when a player lands on a chance cell.
     * It will randomly draw one event from the event database and call the call() function from the Event class.
     *
     */
    CellActivations activate();
};
