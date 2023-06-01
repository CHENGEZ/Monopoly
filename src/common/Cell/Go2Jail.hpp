#pragma  once
#include "Cell.hpp"

class Go2Jail : public Cell
{
public:
    /*
     * Public constructor for deserialization and creation
     */
    Go2Jail(const int ID, const string NAME, vector<std::string> playerIDsOnCell);

    /**
     * @brief Call back function when a player lands on it.
     * Simply sends the player that landed on it to jail.
     *
     */
    CellActivations activate();
};
