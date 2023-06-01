//
// Created by Manuel on 29.01.2021.
//

#include "UseGetOutOfJailFreeRequest.h"

// Public constructor
UseGetOutOfJailFreeRequest::UseGetOutOfJailFreeRequest(std::string game_id, std::string player_id)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::USE_GET_OUT_OF_JAIL_FREE, uuid_generator::generate_uuid_v4(), player_id,
                                                 game_id) )
{ }

// private constructor for deserialization
UseGetOutOfJailFreeRequest::UseGetOutOfJailFreeRequest(ClientRequest::BaseClassProperties props) :
        ClientRequest(props)
{ }

UseGetOutOfJailFreeRequest* UseGetOutOfJailFreeRequest::fromJson(const rapidjson::Value &json) {
    return new UseGetOutOfJailFreeRequest(ClientRequest::extractBaseClassProperties((json)));
}

void UseGetOutOfJailFreeRequest::writeIntoJson(rapidjson::Value &json,
                                    rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
}