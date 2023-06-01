//
// Created by Manuel on 29.01.2021.
//
// The GameInstanceManager only exists on the server side. It stores all currently active games and offers
// functionality to retrieve game instances by id and adding players to games.
// If a new player requests to join a game but no valid GameInstance is available, then this class
// will generate a new GameInstance and add it to the unordered_map of (active) game instances.

#ifndef MONOPOLY_GAME_INSTANCE_MANAGER_H
#define MONOPOLY_GAME_INSTANCE_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "GameInstance.h"

class GameInstanceManager {

private:

    inline static std::shared_mutex gamesLutLock;
    static std::unordered_map<std::string, GameInstance*> gamesLut;

    static GameInstance* createNewGame();
    static GameInstance* findJoinableGameInstance();

public:

    // returns true if the desired GameInstance 'gameId' was found or false otherwise.
    // The found game instance is written into gameInstancePtr.
    static bool tryGetGameInstance(const std::string& gameId, GameInstance*& gameInstancePtr);
    // returns true if the desired player 'playerId' was found and is connected to a GameInstance.
    // The found player and GameInstance will be written into 'player' and 'gameInstancePtr'
    static bool tryGetPlayerAndGameInstance(const std::string& playerId, Player*& player, GameInstance*& gameInstancePtr, std::string& err);

    // Try to add 'player' to any game. Returns true if 'player' is successfully added to a GameInstance.
    // The joined GameInstance will be written into 'gameInstancePtr'.
    static bool tryAddPlayerToAnyGame(Player* player, GameInstance*& gameInstancePtr, std::string& err);
    // Try to add 'player' to the provided 'gameInstancePtr'. Returns true if success and false otherwise.
    static bool tryAddPlayer(Player* player, GameInstance*& gameInstancePtr, std::string& err);


    static bool tryRemovePlayer(Player* player, const std::string& gameId, std::string& err);
    static bool tryRemovePlayer(Player* player, GameInstance*& gameInstancePtr, std::string& err);

};


#endif //MONOPOLY_GAME_INSTANCE_MANAGER_H
