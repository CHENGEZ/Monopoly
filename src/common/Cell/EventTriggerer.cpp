#include "EventTriggerer.hpp"

CommunityChest::CommunityChest(const int ID, const string NAME,vector<std::string> playerIDsOnCell)
: Cell(ID, NAME, playerIDsOnCell)
{
    cellType = COMMUNITY_CHEST;
}

Chance::Chance(const int ID, const string NAME,vector<std::string> playerIDsOnCell)
: Cell(ID, NAME, playerIDsOnCell)
{
    cellType = CHANCE;
}

CellActivations CommunityChest::activate()
{
    return EVENT_TRIGGER_COMMUNITY;
}


CellActivations Chance::activate()
{
    return EVENT_TRIGGER_CHANCE;
}

