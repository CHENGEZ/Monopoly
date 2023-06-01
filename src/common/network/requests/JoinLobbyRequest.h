//
// Created by Manuel on 29.01.2021.
//

#ifndef MONOPOLY_JOIN_LOBBY_REQUEST_H
#define MONOPOLY_JOIN_LOBBY_REQUEST_H


#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class JoinLobbyRequest : public ClientRequest{

private:
    std::string _playerName;

    static std::string undefinedGameId;
    /*
     * Private constructor for deserialization
     */
    JoinLobbyRequest(BaseClassProperties, std::string name);

public:

    [[nodiscard]] std::string getPlayerName() const { return this->_playerName; }
    /*
     * Constructor to join any game
     */
    JoinLobbyRequest(std::string playerId, std::string name);

    /*
     * Constructor to join a specific game
     */
    JoinLobbyRequest(std::string gameId, std::string playerId, std::string name);

    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static JoinLobbyRequest* fromJson(const rapidjson::Value& json);
};


#endif //MONOPOLY_JOIN_LOBBY_REQUEST_H
