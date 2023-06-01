//
// Created by Manuel on 29.01.2021.
//
// The GameInstanceManager only exists on the server side. It stores all currently active games and offers
// functionality to retrieve game instances by id and adding players to games.
// If a new player requests to join a game but no valid GameInstance is available, then this class
// will generate a new GameInstance and add it to the unordered_map of (active) game instances.

#include "GameInstanceManager.h"

#include "PlayerManager.h"
#include "ServerNetworkManager.h"

// Initialize static map
std::unordered_map<std::string, GameInstance*> GameInstanceManager::gamesLut = {};

GameInstance *GameInstanceManager::findJoinableGameInstance() {
    std::vector<std::string> toRemove;
    GameInstance* res = nullptr;
    gamesLutLock.lock_shared();
    for (auto it = gamesLut.begin(); it != gamesLut.end(); it++) {
        if (it->second->isFinished()) {    // also check if there are any finished games that can be removed
            toRemove.push_back(it->first);
        } else if (!it->second->isFull() && !it->second->isStarted()) {
            res = it->second; // found a non-full, non-started game
            break;
        }
    }
    gamesLutLock.unlock_shared();

    if (res == nullptr) {
        // couldn't find a non-full, non-started game -> create a new one
        res = createNewGame();
    }

    // remove all finished games
    if (toRemove.size() > 0) {
        gamesLutLock.lock();
        for (auto& gameId : toRemove) {
            gamesLut.erase(gameId);
        }
        gamesLutLock.unlock();
    }
    return res;
}

GameInstance* GameInstanceManager::createNewGame() {
    GameInstance* newGame = new GameInstance();
    gamesLutLock.lock();  // exclusive
    GameInstanceManager::gamesLut.insert({newGame->getId(), newGame});
    gamesLutLock.unlock();
    return newGame;
}


bool GameInstanceManager::tryGetGameInstance(const std::string& gameId, GameInstance *&gameInstancePtr) {
    gameInstancePtr = nullptr;
    gamesLutLock.lock_shared();
    auto it = GameInstanceManager::gamesLut.find(gameId);
    if (it != gamesLut.end()) {
        gameInstancePtr = it->second;
    }
    gamesLutLock.unlock_shared();
    return gameInstancePtr != nullptr;
}

bool
GameInstanceManager::tryGetPlayerAndGameInstance(const std::string& playerId, Player *&player, GameInstance *&gameInstancePtr, std::string& err) {
    if (PlayerManager::tryGetPlayer(playerId, player)) {
        if (GameInstanceManager::tryGetGameInstance(player->getGameId(), gameInstancePtr)) {
            return true;
        } else {
            err = "Could not find gameId" + player->getGameId() + " associated with this player";
        }
    } else {
        err = "Could not find requested player " + playerId + " in database.";
    }
    return false;
}


bool GameInstanceManager::tryAddPlayerToAnyGame(Player *player, GameInstance*& gameInstancePtr, std::string& err) {

    // check that player is not already subscribed to another game
    if (player->getGameId() != "") {
        if (gameInstancePtr != nullptr && player->getGameId() != gameInstancePtr->getId()) {
            err = "Could not join game with id " + gameInstancePtr->getId() + ". Player is already active in a different game with id " + player->getGameId();
        } else {
            err = "Could not join game. Player is already active in a game";
        }
        return false;
    }

    if (gameInstancePtr == nullptr) {
        // Join any non-full, non-started game
        for (int i = 0; i < 10; i++) {
            // make at most 10 attempts of joining a src (due to concurrency, the game could already be full or started by the time
            // tryAddPlayerToAnyGame() is invoked) But with only few concurrent requests it should succeed in the first iteration.
            gameInstancePtr = findJoinableGameInstance();
            if (tryAddPlayer(player, gameInstancePtr, err)) {
                return true;
            }
        }
        return false;
    }
    else {
        return tryAddPlayer(player, gameInstancePtr, err);
    }
}


bool GameInstanceManager::tryAddPlayer(Player *player, GameInstance *&gameInstancePtr, std::string& err) {
    if (player->getGameId() != "") {
        if (player->getGameId() != gameInstancePtr->getId()) {
            err = "Player is already active in a different src with id " + player->getGameId();
        } else {
            err = "Player is already active in this src";
        }
        return false;
    }

    if (gameInstancePtr->tryAddPlayer(player, err)) {
        player->setGameId(gameInstancePtr->getId());   // mark that this player is playing in a src
        return true;
    } else {
        return false;
    }
}

bool GameInstanceManager::tryRemovePlayer(Player *player, const std::string& gameId, std::string &err) {
    GameInstance* gameInstancePtr = nullptr;
    if (tryGetGameInstance(gameId, gameInstancePtr)) {
        return tryRemovePlayer(player, gameInstancePtr, err);
    } else {
        err = "The requested src could not be found. Requested src id was " + gameId;
        return false;
    }
}

bool GameInstanceManager::tryRemovePlayer(Player *player, GameInstance *&gameInstancePtr, std::string &err) {
    return gameInstancePtr->tryRemovePlayer(player, err);
}

