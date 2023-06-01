//
// Created by Ricky on 2023/5/6.
//

#ifndef MONOPOLY_CLIENTCOMMUNICATIONTOOLBOX_H
#define MONOPOLY_CLIENTCOMMUNICATIONTOOLBOX_H

#include <string>
#include "../../common/game_state/player/Player.hpp"
#include "../../common/game_state/Game.h"


class ClientCommunicationToolbox {

    // This function used to connectToServer the server

public:
    static Player * connectToServer(const std::string& iP, const int port, const std::string& Name);

    static bool rollDiceRequest(string gameID, string playerID);
    static bool startGame(Game* currGame, Player* currPlayer);
    static bool buyBuilding(string gameID, string playerID, string cellID);
    static bool buyProperty(string gameID, string playerID, string cellID);
    static bool endTurn(string gameID, string playerID);
    static bool sellBuilding(string gameID, string playerID, string cellID);
    static bool sellProperty(string gameID, string playerID, string cellID);
    static bool useGetOutOfJailFree(string gameID, string playerID);




};


#endif //MONOPOLY_CLIENTCOMMUNICATIONTOOLBOX_H
