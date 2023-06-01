//
// Created by Manuel on 29.01.2021.
//

#include "EndTurnRequest.h"

// Public constructor
EndTurnRequest::EndTurnRequest(std::string gameId, std::string playerId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::END_TURN, uuid_generator::generate_uuid_v4(), playerId,
                                                 gameId) )
{ }

// private constructor for deserialization
EndTurnRequest::EndTurnRequest(ClientRequest::BaseClassProperties props) :
        ClientRequest(props)
{ }

EndTurnRequest* EndTurnRequest::fromJson(const rapidjson::Value &json) {
    return new EndTurnRequest(ClientRequest::extractBaseClassProperties((json)));
}

void EndTurnRequest::writeIntoJson(rapidjson::Value &json,
                                      rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
}