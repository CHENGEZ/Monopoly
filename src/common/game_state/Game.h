//
// Created by Ricky on 2023/5/6.
//

#ifndef MONOPOLY_GAME_H
#define MONOPOLY_GAME_H

#include <vector>
#include <algorithm>
#include <string>
#include "../../../rapidjson/include/rapidjson/document.h"
#include "player/Player.hpp"
#include "events/Event.h"
#include "EventExecutor.h"
#include "../Cell/Cell.hpp"
#include "../Cell/Avenue.hpp"
#include "../Cell/Go2Jail.hpp"
#include "../Cell/EventTriggerer.hpp"
#include "../Cell/Tax.hpp"
#include "../Cell/Railway.hpp"
#include "../Cell/Jail.hpp"
#include "../Cell/GoAndFreeParking.hpp"
#include "../Cell/Utility.hpp"

#include "../serialization/unique_serializable.h"
#include "../serialization/vector_utils.h"

#define DEVELOPER_TESTING 0
// The game status is most basic background of our game, and it should be consistent for all players
class Game : public unique_serializable
{
#if DEVELOPER_TESTING
public:
    static const int _maxNoPlayers = 6;
    static const int _minNoPlayers = 2;

    static const int _maxRounds = 200;

    std::vector<Player *> _players;
    std::vector<Event *> _chanceEvents;
    std::vector<Event *> _communityChestEvents;
    std::vector<Event *> _executedChanceEvents = std::vector<Event *>();
    std::vector<Event *> _executedCommunityChestEvents = std::vector<Event *>();
    std::vector<Cell *> _cells;
    bool _isStarted;
    bool _isFinished;
    int _roundNumber;
    std::string _currentPlayerIdx; // turnNumber in old code
    std::string _startingPlayerIdx;
    std::string _delta;
#else
private:
    static const int _maxNoPlayers = 6;
    static const int _minNoPlayers = 2;

    static const int _maxRounds = 200;

    std::vector<Player *> _players;
    std::vector<Event *> _chanceEvents;
    std::vector<Event *> _communityChestEvents;
    std::vector<Event *> _executedChanceEvents = std::vector<Event *>();
    std::vector<Event *> _executedCommunityChestEvents = std::vector<Event *>();
    std::vector<Cell *> _cells;
    bool _isStarted;
    bool _isFinished;
    int _roundNumber;
    std::string _currentPlayerIdx; // turnNumber in old code
    std::string _startingPlayerIdx;
    std::string _delta;
#endif

private:
    // from_diff constructor
    Game(std::string id);
    EventExecutor *eventExecutor;

    // Constructor for deserialization
    Game(
        std::string id,
        std::vector<Player *> &players,
        std::vector<Event *> &chanceEvents,
        std::vector<Event *> &communityChestEvents,
        std::vector<Cell *> &cells,
        bool isStarted,
        bool isFinished,
        int roundNumber,
        std::string currentPlayerIdx,
        std::string startingPlayerIdx,
        std::string delta);


    void cellActivate(CellActivations action, int cellID, std::string &err);
    void executeEvent(EventCommand command, int cellID, std::string &err);
    bool nameAlreadyUsed(Player *player, std::string name) const;
    bool existPlayerOwnCommunityFreeJail() const;
    bool existPlayerOwnChanceFreeJail() const;


public:
    Game();
    ~Game();

    // Accessors
    bool isFull() const;
    bool isStarted() const;
    bool isFinished() const;
    bool isPlayerInGame(Player *player) const;
    bool isAllowedToPlayNow(Player *player) const;
    std::vector<Player *> &getPlayers();
    std::vector<Cell*>& getCells();
    int getRoundNumber() const;

    std::string getDelta() const{return _delta;};

    Player *getPlayerFromId(std::string id) const;

    Player *getCurrentPlayer() const;

#ifdef MONOPOLY_SERVER

    // The part that should only vaild in server
    bool removePlayer(Player *player, std::string &err);
    bool addPlayer(Player *player, std::string &err);
    bool startGame(std::string &err);

    // Player initiated actions
    bool buyProperty(Player *player, const int cellId, std::string &err);
    bool sellProperty(Player *player, const int cellId, std::string &err);
    bool buyBuilding(Player *player, const int cellId, std::string &err);
    bool sellBuilding(Player *player, const int cellId, std::string &err);
    bool rollDice(Player *player, std::string &err);
    bool endTurn(Player *player, std::string &err);
    bool useGetOutOfJailCard(Player *player, std::string &err);

    // Server initiated movement actions
    void movePlayer(Player *player, int steps, std::string &err);
    void movePlayerTo(Player *player, const int cellId, std::string &err);
    void movePlayerToJail(Player *player, std::string &err);
    void movePlayerToNearestRailroad(Player *player, std::string &err);
    void movePlayerToStart(Player *player, std::string &err);

    // Server initiated money actions
    void givePlayerMoney(Player *player, int amount);
    void removePlayerMoney(Player *player, int amount);

    // Server initiated property actions
    void givePlayerProperty(Player *player, const int cellId);
    void takePlayerProperty(Player *player, const int cellId);


#endif

    // serializable interface
    void writeIntoJson(rapidjson::Value &json, rapidjson::Document::AllocatorType &allocator) const override;
    static Game *fromJson(const rapidjson::Value &json);
};

#endif // MONOPOLY_GAME_H
