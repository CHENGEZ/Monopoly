//
// Created by Manuel on 25.01.2021.
//
// The GameInstance class is a wrapper around the Game of an active instance of the game.
// This class contains functions to modify the contained game_state.

#ifndef MONOPOLY_GAME_INSTANCE_H
#define MONOPOLY_GAME_INSTANCE_H

#include <vector>
#include <string>
#include <mutex>
#include "../common/game_state/player/Player.hpp"
#include "../common/game_state/Game.h"

class GameInstance {
private:
    Game* _gameState;
    bool isPlayerAllowedToPlay(Player* player);
    inline static std::mutex modificationLock;

public:
    GameInstance();
    ~GameInstance();
    std::string getId();

    Game* getGameState();

    bool isFull();
    bool isStarted();
    bool isFinished();

    // game update functions
    bool startGame(Player* player, std::string& err);
    bool tryAddPlayer(Player* newPlayer, std::string& err);
    bool tryRemovePlayer(Player* player, std::string& err);

    bool rollDice(Player* player, std::string& err);
    bool buyProperty(Player* player, int cellId, std::string& err);
    bool sellProperty(Player* player, int cellId, std::string& err);
    bool buyBuilding(Player* player, int cellId, std::string& err);
    bool sellBuilding(Player* player, int cellId, std::string& err);
    bool useGetOutOfJailCard(Player* player, std::string& err);
    bool endTurn(Player* player, std::string& err);

};


#endif //MONOPOLY_GAME_INSTANCE_H

