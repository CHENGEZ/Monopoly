#pragma  once
#include "Cell.hpp"

class Jail : public Cell
{
public:
    /*
     * Public constructor for deserialization and creation
     */
    Jail(const int ID, const string NAME, vector<std::string> playerIDsOnCell);

    /**
     * @brief Call back function when a player lands on it. But it will just be a dummy function with
     * empty implementation since landing on it means a player is “just visiting”,
     * it does not mean this player is “in jail”.
     *
     */
    CellActivations activate();
};
