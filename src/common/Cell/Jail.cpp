#include "Jail.hpp"

Jail::Jail(const int ID, const string NAME, vector<std::string> playerIDsOnCell)
: Cell(ID, NAME, playerIDsOnCell)
{
    cellType = JAIL;
}

CellActivations Jail::activate()
{
    return DO_NOTHING;
}

