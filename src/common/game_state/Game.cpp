//
// Created by Ricky on 2023/5/6.
//

#include "Game.h"
#include "../exceptions/MonopolyException.hpp"

#include <iostream>

int randint(int min, int max) // both inclusive
{
    srand((unsigned)time(0));
    return (rand() % (max - min + 1)) + min;
}

// Base constructor, assign everything
// Game::Game()
//{
//}

Player *Game::getPlayerFromId(std::string id) const
{
    //    cout << "game output" << endl;
    for (auto &player : this->_players)
    {
        //        cout << player->getId() << endl;
        if (player->getId() == id)
        {
            return player;
        }
    }
    throw std::invalid_argument("Player with id " + id + " not found");
}

Game::Game() : unique_serializable()
{
    this->_players = std::vector<Player *>();
    this->_chanceEvents = std::vector<Event *>();
    this->_communityChestEvents = std::vector<Event *>();
    this->_cells = std::vector<Cell *>();
    this->_isStarted = false;
    this->_isFinished = false;
    this->_currentPlayerIdx = "";
    this->_startingPlayerIdx = "";
    this->_roundNumber = 0;
    this->_delta = "";
}

Game::Game(std::string id) : unique_serializable(id) // from_diff constructor
{
    this->_players = std::vector<Player *>();
    this->_chanceEvents = std::vector<Event *>();
    this->_communityChestEvents = std::vector<Event *>();
    this->_cells = std::vector<Cell *>();
    this->_isStarted = false;
    this->_isFinished = false;
    this->_currentPlayerIdx = "";
    this->_roundNumber = 0;
    this->_startingPlayerIdx = "";
    this->_delta = "";
}

Game::Game(std::string id,
           std::vector<Player *> &players,
           std::vector<Event *> &chanceEvents,
           std::vector<Event *> &communityChestEvents,
           std::vector<Cell *> &cells,
           bool isStarted,
           bool isFinished,
           int roundNumber,
           std::string currentPlayerIdx,
           std::string startingPlayerIdx,
           std::string delta) : unique_serializable(id) // constructor for deserialization
{
    this->_chanceEvents = chanceEvents;
    this->_communityChestEvents = communityChestEvents;
    this->_cells = cells;
    this->_players = players;
    this->_isStarted = isStarted;
    this->_isFinished = isFinished;
    this->_currentPlayerIdx = currentPlayerIdx;
    this->_roundNumber = roundNumber;
    this->_startingPlayerIdx = startingPlayerIdx;
    this->_delta = delta;
}

Game::~Game()
{
    for (int i = 0; i < this->_players.size(); i++)
    {
        delete this->_players[i];
    }
    for (int i = 0; i < this->_chanceEvents.size(); i++)
    {
        delete this->_chanceEvents[i];
    }
    for (int i = 0; i < this->_communityChestEvents.size(); i++)
    {
        delete this->_communityChestEvents[i];
    }
    for (int i = 0; i < this->_cells.size(); i++)
    {
        delete this->_cells[i];
    }
}
bool Game::isFull() const { return this->_players.size() == this->_maxNoPlayers; }
bool Game::isStarted() const { return this->_isStarted; }
bool Game::isFinished() const { return this->_isFinished; }
bool Game::isPlayerInGame(Player *player) const { return std::find(this->_players.begin(), this->_players.end(), player) != this->_players.end(); }
bool Game::isAllowedToPlayNow(Player *player) const { return player->getId() == _currentPlayerIdx; }
std::vector<Player *> &Game::getPlayers()
{
    return _players;
}
int Game::getRoundNumber() const
{
    return this->_roundNumber;
}
#ifdef MONOPOLY_SERVER
void Game::cellActivate(CellActivations action, int cellID, std::string &err)
{
    Player *payer;
    Player *receiver;
    int amount = 0;
    int randomIndex;
    Event *randomEvent;
    int numRailwayOwned, numUtilityOwned;

    switch (action)
    {
    case DO_NOTHING:
        break;

    case P2P_MONEY_AVENUE:
        payer = getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back());
        receiver = getPlayerFromId(this->_cells[cellID]->getOwnerId());
        amount = this->_cells[cellID]->getActualRent();
        removePlayerMoney(payer, amount);
        givePlayerMoney(receiver, amount);
        err += "\n" + payer->getName() + " paid " + receiver->getName() + "$" + std::to_string(amount);
        this->_delta = err;
        break;

    case P2P_MONEY_RAILWAY:
        payer = getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back());
        receiver = getPlayerFromId(this->_cells[cellID]->getOwnerId());
        numRailwayOwned = count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 5) +
                          count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 15) +
                          count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 25) +
                          count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 35);
        amount = numRailwayOwned * this->_cells[cellID]->getBaseRent();
        removePlayerMoney(payer, amount);
        givePlayerMoney(receiver, amount);
        err += "\n" + payer->getName() + " paid " + receiver->getName() + "$" + std::to_string(amount);
        this->_delta = err;
        break;

    case P2C_MONEY_TAX:
        payer = getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back());
        amount = this->_cells[cellID]->getTaxAmount();
        removePlayerMoney(payer, amount);
        break;

    case P2P_MONEY_UTILITY:
        payer = getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back());
        receiver = getPlayerFromId(this->_cells[cellID]->getOwnerId());
        numUtilityOwned = count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 12) +
                          count(receiver->getPropertiesOwnedIds()->begin(), receiver->getPropertiesOwnedIds()->end(), 28);
        switch (numUtilityOwned)
        {
        case 1:
            amount = 4 * payer->getLastRolledDice();
            break;
        case 2:
            amount = 10 * payer->getLastRolledDice();
            break;
        default:
            break;
        }
        removePlayerMoney(payer, amount);
        givePlayerMoney(receiver, amount);
        err += "\n" + payer->getName() + " paid " + receiver->getName() + "$" + std::to_string(amount);
        this->_delta = err;
        break;

    case EVENT_TRIGGER_COMMUNITY:
        // randomly draw an event from _communityChestEvents
        randomIndex = randint(0, this->_communityChestEvents.size() - 1);
        randomEvent = this->_communityChestEvents[randomIndex];
        err += "\n" + randomEvent->getDescription();
        this->_delta = err;
        executeEvent(this->eventExecutor->commandExecuteEvent(randomEvent), cellID, err);
        this->_communityChestEvents.erase(std::remove(this->_communityChestEvents.begin(), this->_communityChestEvents.end(), randomEvent), this->_communityChestEvents.end());
        this->_executedCommunityChestEvents.emplace_back(randomEvent);
        if (randomEvent->getType() == Event::GET_OUT_OF_JAIL_FREE)
            getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back())->setOwnCommunityChestGetOutOfJail(true);
        break;

    case EVENT_TRIGGER_CHANCE:
        // randomly draw an event from _communityChestEvents
        randomIndex = randint(0, this->_chanceEvents.size() - 1);
        randomEvent = this->_chanceEvents[randomIndex];
        err += "\n" + randomEvent->getDescription();
        this->_delta = err;
        executeEvent(this->eventExecutor->commandExecuteEvent(randomEvent), cellID, err);
        this->_chanceEvents.erase(std::remove(this->_chanceEvents.begin(), this->_chanceEvents.end(), randomEvent), this->_chanceEvents.end());
        this->_executedChanceEvents.emplace_back(randomEvent);
        if (randomEvent->getType() == Event::GET_OUT_OF_JAIL_FREE)
            getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back())->setOwnChanceGetOutOfJail(true);
        break;

    case GO_TO_JAIL:
        movePlayerToJail(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), err);
        break;

    default:
        break;
    }
}

void Game::executeEvent(EventCommand command, int cellID, std::string &err)
{
    string dummy;
    Player *player = nullptr;
    int building_num = 0;
    switch (command.type)
    {
    case P2C_MONEY:
        removePlayerMoney(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), command.value);
        break;
    case C2P_MONEY:
        givePlayerMoney(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), command.value);
        break;
    case P2P_MONEY:
        removePlayerMoney(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), command.value * (_players.size() - 1));
        for (int i = 0; i < _players.size(); i++)
        {
            if (_players[i]->getId() != this->_cells[cellID]->getPlayerOnCell().back())
            {
                givePlayerMoney(_players[i], command.value);
            }
        }
        break;
    case GO_TO_ABS:
        if (command.value == 10)
            movePlayerToJail(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), err);
        else
            movePlayerTo(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), command.value, err);
        break;
    case GO_TO_REL:
        movePlayerTo(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), cellID + command.value, err);
        break;
    case MOVE_TO_NEAREST_RAIL: // This requires paying double
        movePlayerToNearestRailroad(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), err);
        movePlayerToNearestRailroad(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), dummy);
        break;
    case GET_OUT_OF_JAIL_FREE_CARD:
        getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back())->setNumGetOutOfJailFreeCard(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back())->getNumGetOutOfJailFreeCard() + 1);
        break;
    case GENERAL_REPAIRS:
        player = getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back());
        removePlayerMoney(player, player->getNumOwnedBuildings() * command.value);
        break;
    case STREET_REPAIRS:
        for (int i = 0; i < _players.size(); i++)
        {
            building_num += _players[i]->getNumOwnedBuildings();
        }
        removePlayerMoney(getPlayerFromId(this->_cells[cellID]->getPlayerOnCell().back()), building_num * command.value);
        break;
    default:
        break;
    }
}

bool Game::nameAlreadyUsed(Player *player, std::string name) const
{
    for (int i = 0; i < this->_players.size(); i++)
    {
        if (_players[i]->getName() == name)
        {
            return true;
        }
    }
    return false;
}

bool Game::buyProperty(Player *player, const int cellId, std::string &err)
{
    assert(!player->isOutOfGame());
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    bool enoughMoney = player->getBalance() >= this->_cells[cellId]->getPrice();
    bool notYetOwned = _cells[cellId]->getOwnerId() == "";
    bool onCell = cellId == player->getPosition();
    bool boughtSuccess = enoughMoney && notYetOwned && onCell;
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to buy properties now because it is not his/her turn!";
        this->_delta = err;
        return false;
    }
    if (boughtSuccess)
    {
        removePlayerMoney(player, this->_cells[cellId]->getPrice());
        givePlayerProperty(player, cellId);
        this->_cells[cellId]->setOwnerId(player->getId());
        err = "Property " + this->_cells[cellId]->getName() + " bought successfully!";
        this->_delta = err;
        return true;
    }
    else
    {
        err = "An error occurred when buying property either because the player doesn't have enough money, the property is already owned, or the player is not on it.";
        this->_delta = err;
        return false;
    }
}

bool Game::sellProperty(Player *player, const int cellId, std::string &err)
{
    assert(!player->isOutOfGame());
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to sell buildings now because it is not his/her turn!";
        this->_delta = err;
        return false;
    }
    bool ownedByThisPlayer = this->_cells[cellId]->getOwnerId() == player->getId();

    if (ownedByThisPlayer)
    {
        givePlayerMoney(player, this->_cells[cellId]->getMortgage());
        takePlayerProperty(player, cellId);
        this->_cells[cellId]->setOwnerId("");
        err = "Property sold successfully!";
        this->_delta = err;
        return true;
    }
    else
    {
        err = "Cannnot sell this property because the player does not own it!";
        this->_delta = err;
        return false;
    }
}

bool Game::buyBuilding(Player *player, const int cellId, std::string &err)
{
    assert(!player->isOutOfGame());
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    bool ownedByThisPlayer = this->_cells[cellId]->getOwnerId() == player->getId();
    bool propertyNotFull = this->_cells[cellId]->getMaxBuildings() > this->_cells[cellId]->getCurrentNumBuildings();
    bool onCell = cellId == player->getPosition();
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to buy buildings now because it is not his/her turn!";
        this->_delta = err;
        return false;
    }
    if (ownedByThisPlayer && propertyNotFull && onCell)
    {
        removePlayerMoney(player, this->_cells[cellId]->getBuildingPrice());
        this->_cells[cellId]->setNumBuildings(this->_cells[cellId]->getCurrentNumBuildings() + 1);
        err = "Building added to " + this->_cells[cellId]->getName() + " sucessfully!";
        this->_delta = err;
        player->setNumOwnedBuildings(player->getNumOwnedBuildings() + 1);
        player->setNetWorth(player->getNetWorth() + this->_cells[cellId]->getBuildingPrice());
        return true;
    }
    else
    {
        err = "Cannot add building to property " +
              this->_cells[cellId]->getName() +
              " either because it's not owned by player " + player->getName() + " or it has reached max buildings";
        this->_delta = err;
        return false;
    }
}
bool Game::sellBuilding(Player *player, const int cellId, std::string &err)
{
    assert(!player->isOutOfGame());
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to sell buildings now because it is not his/her turn!";
        this->_delta = err;
        return false;
    }
    bool ownedByThisPlayer = this->_cells[cellId]->getOwnerId() == player->getId();
    bool buildingExist = this->_cells[cellId]->getCurrentNumBuildings() > 0;

    if (ownedByThisPlayer && buildingExist)
    {
        // The plan is to NOT give player money when removing a building
        // givePlayerMoney(player, this->_cells[cellId]->getBuildingPrice());
        this->_cells[cellId]->setNumBuildings(this->_cells[cellId]->getCurrentNumBuildings() - 1);
        err = "Building removed from " + this->_cells[cellId]->getName() + " sucessfully!";
        this->_delta = err;
        player->setNumOwnedBuildings(player->getNumOwnedBuildings() - 1);
        player->setNetWorth(player->getNetWorth() - this->_cells[cellId]->getBuildingPrice());
        return true;
    }
    else
    {
        err = "Cannot remove building from property" +
              this->_cells[cellId]->getName() +
              " either because it's not owned by player " + player->getName() + " or there is no building to remove.";
        this->_delta = err;
        return false;
    }
}

bool Game::rollDice(Player *player, std::string &err)
{
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to roll dice and play at this moment!";
        this->_delta = err;
        return false;
    }
    if (player->isAlreadyRolled())
    {
        err = "Player " + player->getName() + " has already rolled!";
        this->_delta = err;
        return false;
    }
    if (player->getTurnsLeftInJail() != 0)
    {
        err = "Player " + player->getName() + " is in Jail!";
        this->_delta = err;
        return false;
    }
    int steps1 = randint(1, 6);
    int steps2 = randint(1, 6);
    int steps = steps1 + steps2;
    assert(steps >= 2 && steps <= 12);
    player->setLastRolledDice(steps);
//    steps = 30;
    player->setAlreadyRolled(true);
    err = "Player " + player->getName() + " rolled " + std::to_string(steps) + " and moved forward for that amount.";
    this->_delta = err;
    movePlayer(player, steps, err);
    return true;
}

void Game::givePlayerMoney(Player *player, int amount)
{
    player->setBalance(player->getBalance() + amount);
    player->setNetWorth(player->getNetWorth() + amount);
}
void Game::removePlayerMoney(Player *player, int amount)
{
    player->setBalance(player->getBalance() - amount);
    player->setNetWorth(player->getNetWorth() - amount);
}
void Game::givePlayerProperty(Player *player, const int cellId)
{
    player->addOwnedProperties(cellId);
    int propertyPrice = _cells[cellId]->getPrice();
    player->setNetWorth(player->getNetWorth() + propertyPrice);
}
void Game::takePlayerProperty(Player *player, const int cellId)
{
    player->reduceOwnedProperties(cellId);
    int propertyPrice = _cells[cellId]->getPrice();
    player->setNetWorth(player->getNetWorth() - propertyPrice);
}

void Game::movePlayer(Player *player, int steps, std::string &err)
{
    int totalCells = this->_cells.size();
    int currPos = player->getPosition();
    int tarPos = (currPos + steps) % totalCells;
    this->_cells[currPos]->playerLeave(player);
    this->_cells[tarPos]->playerArrive(player);
    player->setPosition(tarPos);
    if (tarPos < currPos)
        givePlayerMoney(player, 200);
    err += "\nPlayer " + player->getName() + " has been moved to " + this->_cells[tarPos]->getName();
    this->_delta = err;
    cellActivate(this->_cells[tarPos]->activate(), tarPos, err);
}

void Game::movePlayerTo(Player *player, const int cellId, std::string &err)
{
    int currPos = player->getPosition();
    int tarPos = cellId;
    this->_cells[currPos]->playerLeave(player);
    this->_cells[tarPos]->playerArrive(player);
    player->setPosition(cellId);
    if (tarPos < currPos)
        givePlayerMoney(player, 200);
    err += "\nPlayer " + player->getName() + " has been moved to " + this->_cells[cellId]->getName();
    this->_delta = err;
    cellActivate(this->_cells[tarPos]->activate(), tarPos, err);
}

void Game::movePlayerToJail(Player *player, std::string &err) // This one does NOT consider pass GO
{
    int currPos = player->getPosition();
    int tarPos = 10;
    this->_cells[currPos]->playerLeave(player);
    this->_cells[tarPos]->playerArrive(player);
    player->setPosition(tarPos);   // The constant ID for the jail cell should be 10 (i.e. the 11th cell)
    player->setTurnsLeftInJail(3); // The constant number of rounds the players has to wait in Jail
    err += "\nPlayer " + player->getName() + " has been moved to jail";
    this->_delta = err;
    cellActivate(this->_cells[tarPos]->activate(), tarPos, err);
}

void Game::movePlayerToNearestRailroad(Player *player, std::string &err)
{
    // railwayIDs = {5, 15, 25, 35};
    int currPos = player->getPosition();
    int tarPos = -1;
    if (currPos <= 5)
        tarPos = 5;
    else if (currPos <= 15)
        tarPos = 15;
    else if (currPos <= 25)
        tarPos = 25;
    else
        tarPos = 35;

    player->setPosition(tarPos);
    if (tarPos < currPos)
        givePlayerMoney(player, 200);
    this->_cells[currPos]->playerLeave(player);
    this->_cells[tarPos]->playerArrive(player);
    err += "\nPlayer " + player->getName() + " has been moved to " + this->_cells[tarPos]->getName();
    this->_delta = err;
    cellActivate(this->_cells[tarPos]->activate(), tarPos, err);
}

void Game::movePlayerToStart(Player *player, std::string &err)
{
    int currPos = player->getPosition();
    int tarPos = 0;
    player->setPosition(tarPos);
    if (tarPos < currPos)
        givePlayerMoney(player, 200);
    this->_cells[currPos]->playerLeave(player);
    this->_cells[tarPos]->playerArrive(player);
    err += "\nPlayer " + player->getName() + " has been moved to GO";
    this->_delta = err;
    cellActivate(this->_cells[tarPos]->activate(), tarPos, err);
}

bool Game::removePlayer(Player *player, std::string &err)
{
    for (int i = 0; i < this->_players.size(); i++)
    {
        if (this->_players[i]->getId() == player->getId())
        {
            this->_players.erase(this->_players.begin() + i);
            err = "Player " + player->getName() + " has been removed from the game";
            this->_delta = err;
            return true;
        }
    }
    err = "Player " + player->getName() + " is not in the game";
    this->_delta = err;
    return false;
}

bool Game::addPlayer(Player *player, std::string &err)
{
    if (this->_players.size() >= _maxNoPlayers)
    {
        err = "Cannot add player " + player->getName() + " because the game is full";
        this->_delta = err;
        return false;
    }
    else if (nameAlreadyUsed(player, player->getName()))
    {
        err = "Cannot add player " + player->getName() + " because the name is already used";
        this->_delta = err;
        return false;
    }
    else
    {
        this->_players.push_back(player);
        err = "Player " + player->getName() + " has been added to the game";
        this->_delta = err;
        return true;
    }
}

bool Game::endTurn(Player *player, std::string &err)
{
    if (!isAllowedToPlayNow(player))
    {
        err = "This player is not allowed to end turn and play at this moment!";
        this->_delta = err;
        return false;
    }
    else if (!player->isAlreadyRolled() and player->getTurnsLeftInJail() == 0)
    {
        err = "This player hasn't rolled dice yet!";
        this->_delta = err;
        return false;
    }
    // if Event database used up, reboot
    if (this->_chanceEvents.size() == 0)
    {
        if (!existPlayerOwnChanceFreeJail())
        {
            this->_chanceEvents = this->_executedChanceEvents;
            this->_executedChanceEvents.clear();
        }
        else
        {
            Event *freeJailCardEvent = nullptr;
            for (int i = 0; i < this->_executedChanceEvents.size(); ++i)
            {
                if (this->_executedChanceEvents[i]->getType() == Event::GET_OUT_OF_JAIL_FREE)
                {
                    freeJailCardEvent = this->_executedChanceEvents[i];
                    break;
                }
            }
            this->_executedChanceEvents.erase(std::remove(this->_executedChanceEvents.begin(), this->_executedChanceEvents.end(), freeJailCardEvent), this->_executedChanceEvents.end());
            this->_chanceEvents = this->_executedChanceEvents;
            this->_executedChanceEvents.clear();
            this->_executedChanceEvents.emplace_back(freeJailCardEvent);
        }
    }
    if (this->_communityChestEvents.size() == 0)
    {
        if (!existPlayerOwnCommunityFreeJail())
        {
            this->_communityChestEvents = this->_executedCommunityChestEvents;
            this->_executedCommunityChestEvents.clear();
        }
        else
        {
            Event *freeJailCardEvent = nullptr;
            for (int i = 0; i < this->_executedCommunityChestEvents.size(); ++i)
            {
                if (this->_executedCommunityChestEvents[i]->getType() == Event::GET_OUT_OF_JAIL_FREE)
                {
                    freeJailCardEvent = this->_executedCommunityChestEvents[i];
                    break;
                }
            }
            this->_executedCommunityChestEvents.erase(std::remove(this->_executedCommunityChestEvents.begin(), this->_executedCommunityChestEvents.end(), freeJailCardEvent), this->_executedCommunityChestEvents.end());
            this->_communityChestEvents = this->_executedCommunityChestEvents;
            this->_executedCommunityChestEvents.clear();
            this->_executedCommunityChestEvents.emplace_back(freeJailCardEvent);
        }
    }

    // get the current player's index
    player->setAlreadyRolled(false);
    int current_player_index = 0;
    for (; current_player_index < _players.size(); current_player_index = (current_player_index + 1) % _players.size())
    {
        if (_players[current_player_index]->getId() == player->getId())
        {
            break;
        }
    }
    // get next player
    _currentPlayerIdx = _players[(current_player_index + 1) % _players.size()]->getId();
    // remove player if he/she has lost
    if (player->getNetWorth() < 0)
    {
        err = "Player " + player->getName() + " has lost the game";
        this->_delta = err;
        player->setBankrupt(true);
        // update starting player if the player in the starting place is removed
        if (player->getId() == _startingPlayerIdx)
        {
            _startingPlayerIdx = _players[(current_player_index - 1) % _players.size()]->getId();
        }
        removePlayer(player, err);
        // check win condition
        if (_players.size() == 1)
        {
            err = ", Player " + _players[0]->getName() + " has won the game";
            _isFinished = true;
            this->_delta += err;
        }
        return true;
    }
    // if the next player is the starting player, then it's a new round
    if (_currentPlayerIdx == _startingPlayerIdx)
    {
        _roundNumber++;
    }
    // check win condition when max rounds(default=200) is reached
    if (_roundNumber == this->_maxRounds)
    {
        int max_balance = 0;
        int max_balance_player_idx = 0;
        for (int i = 0; i < _players.size(); i++)
        {
            if (_players[i]->getBalance() > max_balance)
            {
                max_balance = _players[i]->getNetWorth();
                max_balance_player_idx = i;
            }
        }
        err = "Player " + _players[max_balance_player_idx]->getName() + " has won the game";
        _isFinished = true;
        this->_delta = err;
        return true;
    }
    // check if the player is in jail
    if (player->getTurnsLeftInJail() > 0)
    {
        player->setTurnsLeftInJail(player->getTurnsLeftInJail() - 1);
        err = "Player " + player->getName() + " has to wait " + std::to_string(player->getTurnsLeftInJail()) + " more rounds in jail";
        this->_delta = err;
        return true;
    }
    else
    {
        err = "Player " + player->getName() + " has ended his turn";
        this->_delta = err;
        return true;
    }
}

bool Game::useGetOutOfJailCard(Player *player, std::string &err)
{
    if (this->_isFinished)
    {
        err = "This game has ended!";
        this->_delta = err;
        return false;
    }
    if (player->getTurnsLeftInJail() == 0)
    {
        err = "You are not in Jail, no need to use 'Get out of Jail free' card.";
        this->_delta = err;
        return false;
    }
    else if (player->getNumGetOutOfJailFreeCard() == 0)
    {
        err = "You do not own any 'Get out of Jail free' card.";
        this->_delta = err;
        return false;
    }
    else if (!isAllowedToPlayNow(player))
    {
        err = "It's not your turn, you can only use your 'Get out of Jail free' card in your turn.";
        this->_delta = err;
        return false;
    }
    else
    {
        player->setNumGetOutOfJailFreeCard(player->getNumGetOutOfJailFreeCard() - 1);
        player->setTurnsLeftInJail(0);
        // check which list of cards should we add to
        if (player->getOwnChanceGetOutOfJail() == true)
        {
            player->setOwnChanceGetOutOfJail(false);
        }
        else if (player->getOwnCommunityChestGetOutOfJail() == true)
        {
            player->setOwnCommunityChestGetOutOfJail(false);
        }
        else
        {
            err = "Bug In Common!";
            this->_delta = err;
            return false;
        }
        err = "Successfully used 'Get out of Jail free' card.";
        this->_delta = err;
        return true;
    }
}

bool Game::startGame(std::string &err)
{
    if (this->_players.size() < _minNoPlayers || this->_players.size() > _maxNoPlayers)
    {
        err = "Cannot start game because the number of players is not in the range of 2 - 6";
        this->_delta = err;
        return false;
    }
    else
    {
        err = "Game has started";
        this->_delta = err;
        // Initialize chanceEvents
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_BROADWALK));
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_GO));
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_ILLINOIS_AVE));
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_ST_CHARLES_PLACE));
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_NEAREST_RAILROAD));
        _chanceEvents.emplace_back(new Event(Event::BANK_PAYS_DIVIDEND));
        _chanceEvents.emplace_back(new Event(Event::GET_OUT_OF_JAIL_FREE));
        _chanceEvents.emplace_back(new Event(Event::GO_BACK_THREE_SPACES));
        _chanceEvents.emplace_back(new Event(Event::GO_TO_JAIL));
        _chanceEvents.emplace_back(new Event(Event::MAKE_GENERAL_REPAIRS));
        _chanceEvents.emplace_back(new Event(Event::SPEEDING_FINE));
        _chanceEvents.emplace_back(new Event(Event::ADVANCE_TO_READING_RAILROAD));
        _chanceEvents.emplace_back(new Event(Event::CHAIRMAN_OF_THE_BOARD));
        _chanceEvents.emplace_back(new Event(Event::BUILDING_LOAN_MATURES));

        // Initialize communityChestEvents
        _communityChestEvents.emplace_back(new Event(Event::ADVANCE_TO_GO));
        _communityChestEvents.emplace_back(new Event(Event::BANK_ERROR_IN_YOUR_FAVOR));
        _communityChestEvents.emplace_back(new Event(Event::DOCTOR_FEE));
        _communityChestEvents.emplace_back(new Event(Event::SALE_OF_STOCK));
        _communityChestEvents.emplace_back(new Event(Event::GET_OUT_OF_JAIL_FREE));
        _communityChestEvents.emplace_back(new Event(Event::GO_TO_JAIL));
        _communityChestEvents.emplace_back(new Event(Event::HOLIDAY_FUND_MATURES));
        _communityChestEvents.emplace_back(new Event(Event::INCOME_TAX_REFUND));
        _communityChestEvents.emplace_back(new Event(Event::IT_IS_YOUR_BIRTHDAY));
        _communityChestEvents.emplace_back(new Event(Event::LIFE_INSURANCE_MATURES));
        _communityChestEvents.emplace_back(new Event(Event::PAY_HOSPITAL_FEES));
        _communityChestEvents.emplace_back(new Event(Event::PAY_SCHOOL_FEES));
        _communityChestEvents.emplace_back(new Event(Event::CONSULTANCY_FEE));
        _communityChestEvents.emplace_back(new Event(Event::STREET_REPAIRS));
        _communityChestEvents.emplace_back(new Event(Event::SECOND_PRIZE_IN_BEAUTY_CONTEST));
        _communityChestEvents.emplace_back(new Event(Event::INHERIT_100));

        // Initialize cells
        _cells.emplace_back(new GoAndFreeParking(0, "Go", vector<std::string>()));
        _cells.emplace_back(new Avenue(1, "Mediterranean Avenue", vector<std::string>(), 60, 2, 4, 3, 50, 30, "", 0));
        _cells.emplace_back(new CommunityChest(2, "Community Chest", vector<std::string>()));
        _cells.emplace_back(new Avenue(3, "Baltic Avenue", vector<std::string>(), 60, 4, 4, 6, 50, 30, "", 0));
        _cells.emplace_back(new Tax(4, "Income Tax", vector<std::string>(), 200));
        _cells.emplace_back(new Railway(5, "Reading Railroad", vector<std::string>(), 200, 100, 50, ""));
        _cells.emplace_back(new Avenue(6, "Oriental Avenue", vector<std::string>(), 100, 6, 4, 10, 50, 50, "", 0));
        _cells.emplace_back(new Chance(7, "Chance", vector<std::string>()));
        _cells.emplace_back(new Avenue(8, "Vermont Avenue", vector<std::string>(), 100, 6, 4, 10, 50, 50, "", 0));
        _cells.emplace_back(new Avenue(9, "Connecticut Avenue", vector<std::string>(), 120, 8, 4, 13, 50, 60, "", 0));
        _cells.emplace_back(new Jail(10, "Jail", vector<std::string>()));
        _cells.emplace_back(new Avenue(11, "St. Charles Place", vector<std::string>(), 140, 10, 4, 17, 100, 70, "", 0));
        _cells.emplace_back(new Utility(12, "Electric Company", vector<std::string>(), 150, 75, ""));
        _cells.emplace_back(new Avenue(13, "States Avenue", vector<std::string>(), 140, 10, 4, 17, 100, 70, "", 0));
        _cells.emplace_back(new Avenue(14, "Virginia Avenue", vector<std::string>(), 160, 12, 4, 20, 100, 80, "", 0));
        _cells.emplace_back(new Railway(15, "Pennsylvania Railroad", vector<std::string>(), 200, 100, 50, ""));
        _cells.emplace_back(new Avenue(16, "St. James Place", vector<std::string>(), 180, 14, 4, 23, 100, 90, "", 0));
        _cells.emplace_back(new CommunityChest(17, "Community Chest", vector<std::string>()));
        _cells.emplace_back(new Avenue(18, "Tennessee Avenue", vector<std::string>(), 180, 14, 4, 23, 100, 90, "", 0));
        _cells.emplace_back(new Avenue(19, "New York Avenue", vector<std::string>(), 200, 16, 4, 27, 100, 100, "", 0));
        _cells.emplace_back(new GoAndFreeParking(20, "Free Parking", vector<std::string>()));
        _cells.emplace_back(new Avenue(21, "Kentucky Avenue", vector<std::string>(), 220, 18, 4, 30, 150, 110, "", 0));
        _cells.emplace_back(new Chance(22, "Chance", vector<std::string>()));
        _cells.emplace_back(new Avenue(23, "Indiana Avenue", vector<std::string>(), 220, 18, 4, 30, 150, 110, "", 0));
        _cells.emplace_back(new Avenue(24, "Illinois Avenue", vector<std::string>(), 240, 20, 4, 34, 150, 120, "", 0));
        _cells.emplace_back(new Railway(25, "B. & O. Railroad", vector<std::string>(), 200, 100, 4, ""));
        _cells.emplace_back(new Avenue(26, "Atlantic Avenue", vector<std::string>(), 260, 22, 4, 37, 150, 130, "", 0));
        _cells.emplace_back(new Avenue(27, "Ventnor Avenue", vector<std::string>(), 260, 22, 4, 37, 150, 130, "", 0));
        _cells.emplace_back(new Utility(28, "Water Works", vector<std::string>(), 150, 75, ""));
        _cells.emplace_back(new Avenue(29, "Marvin Gardens", vector<std::string>(), 280, 24, 4, 40, 150, 140, "", 0));
        _cells.emplace_back(new Go2Jail(30, "Go To Jail", vector<std::string>()));
        _cells.emplace_back(new Avenue(31, "Pacific Avenue", vector<std::string>(), 300, 26, 4, 44, 200, 150, "", 0));
        _cells.emplace_back(new Avenue(32, "North Carolina Avenue", vector<std::string>(), 300, 26, 4, 44, 200, 150, "", 0));
        _cells.emplace_back(new CommunityChest(33, "Community Chest", vector<std::string>()));
        _cells.emplace_back(new Avenue(34, "Pennsylvania Avenue", vector<std::string>(), 320, 28, 4, 47, 200, 160, "", 0));
        _cells.emplace_back(new Railway(35, "Short Line", vector<std::string>(), 200, 100, 50, ""));
        _cells.emplace_back(new Chance(36, "Chance", vector<std::string>()));
        _cells.emplace_back(new Avenue(37, "Park Place", vector<std::string>(), 350, 35, 4, 59, 200, 175, "", 0));
        _cells.emplace_back(new Tax(38, "Luxury Tax", vector<std::string>(), 100));
        _cells.emplace_back(new Avenue(39, "Boardwalk", vector<std::string>(), 400, 50, 4, 85, 200, 200, "", 0));

        // choose a random player to start
        _startingPlayerIdx = _players[randint(0, _players.size() - 1)]->getId();
        _currentPlayerIdx = _startingPlayerIdx;
        _isStarted = true;
        _roundNumber = 1;
        return true;
    }
}
#endif

Player *Game::getCurrentPlayer() const
{
    return getPlayerFromId(_currentPlayerIdx);
}

std::vector<Cell *> &Game::getCells() { return _cells; }

void Game::writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const
{
    unique_serializable::writeIntoJson(json, allocator);

    // _maxNoPlayers
    rapidjson::Value maxNoPlayerValue(rapidjson::kObjectType);
    serializable_value<int> serializableMaxNoPlayer = serializable_value<int>(this->_maxNoPlayers);
    serializableMaxNoPlayer.writeIntoJson(maxNoPlayerValue, allocator);
    json.AddMember("maxNoPlayers", maxNoPlayerValue, allocator);

    // _minNoPlayers
    rapidjson::Value minNoPlayerValue(rapidjson::kObjectType);
    serializable_value<int> serializableMinNoPlayer = serializable_value<int>(this->_minNoPlayers);
    serializableMinNoPlayer.writeIntoJson(minNoPlayerValue, allocator);
    json.AddMember("minNoPlayers", minNoPlayerValue, allocator);

    // _maxRounds
    rapidjson::Value maxRoundValue(rapidjson::kObjectType);
    serializable_value<int> serializableMaxRoundValue = serializable_value<int>(this->_maxRounds);
    serializableMaxRoundValue.writeIntoJson(maxRoundValue, allocator);
    json.AddMember("maxRounds", maxRoundValue, allocator);

    // _players, _chanceEvents, _communityChestEvents, _cells
    json.AddMember("players", vector_utils::serialize_vector(_players, allocator), allocator);
    json.AddMember("chanceEvents", vector_utils::serialize_vector(_chanceEvents, allocator), allocator);
    json.AddMember("communityChestEvents", vector_utils::serialize_vector(_communityChestEvents, allocator), allocator);
    json.AddMember("cells", vector_utils::serialize_vector(_cells, allocator), allocator);

    // _isStarted
    rapidjson::Value isStartedValue(rapidjson::kObjectType);
    serializable_value<bool> serializableIsStartedValue = serializable_value<bool>(this->_isStarted);
    serializableIsStartedValue.writeIntoJson(isStartedValue, allocator);
    json.AddMember("isStarted", isStartedValue, allocator);

    // _isFinished
    rapidjson::Value isFinishedValue(rapidjson::kObjectType);
    serializable_value<bool> serializableIsFinishedValue = serializable_value<bool>(this->_isFinished);
    serializableIsFinishedValue.writeIntoJson(isFinishedValue, allocator);
    json.AddMember("isFinished", isFinishedValue, allocator);

    // _roundNumber
    rapidjson::Value roundNumberValue(rapidjson::kObjectType);
    serializable_value<int> serializableRoundNumber = serializable_value<int>(this->_roundNumber);
    serializableRoundNumber.writeIntoJson(roundNumberValue, allocator);
    json.AddMember("roundNumber", roundNumberValue, allocator);

    // _currentPlayerIdx
    rapidjson::Value currentPlayerIdxValue(rapidjson::kObjectType);
    serializable_value<string> serializablecurrentPlayerIdx = serializable_value<string>(this->_currentPlayerIdx);
    serializablecurrentPlayerIdx.writeIntoJson(currentPlayerIdxValue, allocator);
    json.AddMember("currentPlayerIdx", currentPlayerIdxValue, allocator);

    // _startingPlayerIdx
    rapidjson::Value startingPlayerIdxValue(rapidjson::kObjectType);
    serializable_value<string> serializablestartingPlayerIdx = serializable_value<string>(this->_startingPlayerIdx);
    serializablestartingPlayerIdx.writeIntoJson(startingPlayerIdxValue, allocator);
    json.AddMember("startingPlayerIdx", startingPlayerIdxValue, allocator);

    // _delta
    rapidjson::Value deltaValue(rapidjson::kObjectType);
    serializable_value<string> serializabledelta = serializable_value<string>(this->_delta);
    serializabledelta.writeIntoJson(deltaValue, allocator);
    json.AddMember("delta", deltaValue, allocator);
}

Game *Game::fromJson(const rapidjson::Value &json)
{
    std::string id = json["id"].GetString();

    std::vector<Player *> players;
    const rapidjson::Value &playerArray = json["players"];
    for (rapidjson::SizeType i = 0; i < playerArray.Size(); i++)
    {
        Player *player = Player::fromJson(playerArray[i]);
        players.push_back(player);
    }

    std::vector<Event *> chanceEvents;
    const rapidjson::Value &chanceEventArray = json["chanceEvents"];
    for (rapidjson::SizeType i = 0; i < chanceEventArray.Size(); i++)
    {
        Event *event = Event::fromJson(chanceEventArray[i]);
        chanceEvents.push_back(event);
    }

    std::vector<Event *> communityChestEvents;
    const rapidjson::Value &communityChestEventArray = json["communityChestEvents"];
    for (rapidjson::SizeType i = 0; i < communityChestEventArray.Size(); i++)
    {
        Event *event = Event::fromJson(communityChestEventArray[i]);
        communityChestEvents.push_back(event);
    }

    std::vector<Cell *> cells;
    const rapidjson::Value &cellArray = json["cells"];
    for (rapidjson::SizeType i = 0; i < cellArray.Size(); i++)
    {
        Cell *cell = Cell::fromJson(cellArray[i]);
        cells.push_back(cell);
    }

    bool isStarted = json["isStarted"]["value"].GetBool();
    bool isFinished = json["isFinished"]["value"].GetBool();
    int roundNumber = json["roundNumber"]["value"].GetInt();
    std::string currentPlayerIdx = json["currentPlayerIdx"]["value"].GetString();
    std::string startingPlayerIdx = json["startingPlayerIdx"]["value"].GetString();
    std::string delta = json["delta"]["value"].GetString();

    return new Game(id, players, chanceEvents, communityChestEvents, cells, isStarted, isFinished, roundNumber, currentPlayerIdx, startingPlayerIdx, delta);
}

bool Game::existPlayerOwnChanceFreeJail() const
{
    for (int i = 0; i < this->_players.size(); ++i)
    {
        if (this->_players[i]->getOwnChanceGetOutOfJail() == true)
            return true;
    }
    return false;
}

bool Game::existPlayerOwnCommunityFreeJail() const
{
    for (int i = 0; i < this->_players.size(); ++i)
    {
        if (this->_players[i]->getOwnCommunityChestGetOutOfJail() == true)
            return true;
    }
    return false;
}
