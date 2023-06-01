//
// Created by Manuel on 29.01.2021.
//

#include "StartGameRequest.h"

#ifdef MONOPOLY_SERVER
#include "../../../server/GameInstanceManager.h"
#include "../../../server/GameInstance.h"
#endif

// Public constructor
StartGameRequest::StartGameRequest(std::string gameId, std::string playerId)
        : ClientRequest( ClientRequest::createBaseClassProperties(RequestType::START_GAME, uuid_generator::generate_uuid_v4(), playerId, gameId) )
{ }

// private constructor for deserialization
StartGameRequest::StartGameRequest(ClientRequest::BaseClassProperties props) :
        ClientRequest(props)
{ }

StartGameRequest* StartGameRequest::fromJson(const rapidjson::Value& json) {
    return new StartGameRequest(ClientRequest::extractBaseClassProperties(json));
}

void StartGameRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
}
