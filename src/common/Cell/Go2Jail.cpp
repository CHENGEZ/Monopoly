#include "Go2Jail.hpp"

Go2Jail::Go2Jail(const int ID, const string NAME, vector<std::string> playerIDsOnCell)
: Cell(ID, NAME, playerIDsOnCell)
{
    cellType = GO_2_JAIL;
}

CellActivations Go2Jail::activate()
{
    return GO_TO_JAIL;
}