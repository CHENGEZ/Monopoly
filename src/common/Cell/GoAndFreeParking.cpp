#include "GoAndFreeParking.hpp"

GoAndFreeParking::GoAndFreeParking(const int ID, const string NAME, vector<std::string> playerIDsOnCell)
: Cell(ID, NAME, playerIDsOnCell)
{
    cellType = GO_AND_FREEPARKING;
}

CellActivations GoAndFreeParking::activate()
{
    return DO_NOTHING;
}