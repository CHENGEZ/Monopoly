#ifndef MONOPOLY_GAME_H
#define MONOPOLY_GAME_H

#include <vector>
#include <string>
#include "../../../rapidjson/include/rapidjson/document.h"
#include "player/Player.hpp"
#include "events/Event.h"
#include "../Cell/Cell.hpp"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"

class Game
{
private:
    static const int _maxNofPlayers = 6;
    static const int _minNofPlayers = 2;

    std::vector<Player*> _players;
    std::vector<Event*> _chanceChestEvents;
    std::vector<Event*> _communityChestEvents;
    std::vector<Cell*> _cells;
    std::vector<string*> _scoreboard;
    serializable_value<bool> *_isStarted;
    serializable_value<bool> *_isFinished;
    serializable_value<int> *_roundNumber;
    serializable_value<int> *_currentPlayerIdx; // turnNumber in old code
    serializable_value<int> *_playDirection;    // 1 or -1 depending on which direction is played in
    serializable_value<int> *_startingPlayerIdx;
    serializable_value<string> eventLog;

    // from_diff constructor
    Game(std::string id);

    // deserialization constructor
    Game(
        std::string id,
        std::vector<Player*> &players,
        serializable_value<bool> *isStarted,
        serializable_value<bool> *isFinished,
        serializable_value<int> *currentPlayerIdx,
        serializable_value<int> *playDirection,
        serializable_value<int> *roundNumber,
        serializable_value<int> *startingPlayerIdx,
        std::string eventLog,

        std::vector<Cell*> &cells,
        std::vector<string*> &_scoreboard,
        std::vector<Event*> &chanceChestEvents,
        std::vector<Event*> &communityChestEvents
    );

    // returns the index of 'player' in the '_players' vector
    int getPlayerIndex(Player *player) const;

public:
    Game();
    ~Game();

    // accessors
    bool isFull() const;
    bool isStarted() const;
    bool isFinished() const;
    bool isPlayerInGame(Player *player) const;
    bool isAllowedToPlayNow(Player *player) const;
    std::vector<Player*> &getPlayers(); const
    int getRoundNumber() const;
    std::vector<string> &getScoreboard(); const
    std::string getEventLog(); const


    Event *getCommunityChestEvent() const;
    Event *getChanceChestEvents() const;
    Player *getCurrentPlayer() const;
    std::string getId();

#ifdef MONOPOLY_SERVER
    // server-side state update functions
    // void setupGame(std::string& err);   // server side initialization (Originally setup_round)
    bool removePlayer(Player* player, std::string& err);
    bool addPlayer(Player* player, std::string& err);
    bool startGame(std::string& err);

    //Player initiated actions
    bool buyProperty(Player* player, const std::string& cellId, std::string& err);
    bool sellProperty(Player* player, const std::string& cellId, std::string& err);
    bool buyBuilding(Player* player, const std::string& cellId, std::string& err);
    bool sellBuilding(Player* player, const std::string& cellId, std::string& err);
    bool rollDice(Player* player, std::string& err);
    bool endTurn(Player* player, std::string& err);
    // bool useGetOutOfJailCard(Player* player, std::string& err);

    // Server initiated movement actions
    bool movePlayer(Player* player, int steps, std::string& err);
    bool movePlayerTo(Player* player, const std::string& cellId, std::string& err);
    bool movePlayerToJail(Player* player, std::string& err);
    bool movePlayerToNearestRailroad(Player* player, std::string& err);
    bool movePlayerToStart(Player* player, std::string& err);

    // Server initiated money actions
    bool givePlayerMoney(Player* player, int amount, std::string& err);
    bool removePlayerMoney(Player* player, int amount, std::string& err);

    // Server initiated property actions
    bool givePlayerProperty(Player* player, const std::string& cellId, std::string& err);
    bool takePlayerProperty(Player* player, const std::string& cellId, std::string& err);

    // Server initiated building actions
    bool givePlayerBuilding(Player* player, const std::string& cellId, std::string& err);
    bool takePlayerBuilding(Player* player, const std::string& cellId, std::string& err);

    //Server initiated event actions
    bool drawChanceChestEvent(Player* player, std::string& err); //use EventExecutor::commandExecuteEvent to execute the event
    bool drawCommunityChestEvent(Player* player, std::string& err);

    // end of round functions
    void updateCurrentPlayer(std::string& err);
    void wrapUpRound(std::string& err);

    // event functions
    void drawChanceChestEvent(std::string& err);
    void drawCommunityChestEvent(std::string& err);

    // end of game function
    void gameOver(std::string& err);
#endif

    // serializable interface
    static Game* fromJson(const rapidjson::Value& json);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator);
};

#endif //MONOPOLY_GAME_H