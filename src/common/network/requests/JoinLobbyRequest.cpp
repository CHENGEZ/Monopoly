//
// Created by Manuel on 29.01.2021.
//

#include "JoinLobbyRequest.h"

#ifdef MONOPOLY_SERVER
#include <string>
#include "../../../server/GameInstanceManager.h"
#include "../../../server/PlayerManager.h"
#include "../../../server/GameInstance.h"
#endif

std::string JoinLobbyRequest::undefinedGameId {""};

// Public constructor
JoinLobbyRequest::JoinLobbyRequest(std::string playerId, std::string name)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::JOIN_LOBBY, uuid_generator::generate_uuid_v4(), playerId,
                                                 JoinLobbyRequest::undefinedGameId) ),
          _playerName(name)
{ }

JoinLobbyRequest::JoinLobbyRequest(std::string game_id, std::string playerId, std::string name)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::JOIN_LOBBY, uuid_generator::generate_uuid_v4(), playerId,
                                                 game_id) ),
          _playerName(name)
{ }

// private constructor for deserialization
JoinLobbyRequest::JoinLobbyRequest(ClientRequest::BaseClassProperties props, std::string playerName) :
        ClientRequest(props),
        _playerName(playerName)
{ }

void JoinLobbyRequest::writeIntoJson(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
    rapidjson::Value name_val(_playerName.c_str(), allocator);
    json.AddMember("playerName", name_val, allocator);
}

JoinLobbyRequest* JoinLobbyRequest::fromJson(const rapidjson::Value& json) {
    if (json.HasMember("playerName")) {
        return new JoinLobbyRequest(ClientRequest::extractBaseClassProperties(json), json["playerName"].GetString());
    } else {
        throw MonopolyException("Could not parse JoinLobbyRequest from json. playerName is missing.");
    }
}

