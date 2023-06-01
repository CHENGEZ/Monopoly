//
// Created by Manuel on 28.01.2021.
//

#include "SellBuildingRequest.h"

// Public constructor
SellBuildingRequest::SellBuildingRequest(std::string gameId, std::string player_id, std::string cellId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::SELL_BUILDING, uuid_generator::generate_uuid_v4(),
                                                 player_id, gameId) ),
          _cellId(cellId)
{ }

// private constructor for deserialization
SellBuildingRequest::SellBuildingRequest(ClientRequest::BaseClassProperties props, std::string cellId) :
        ClientRequest(props),
        _cellId(cellId)
{ }

SellBuildingRequest* SellBuildingRequest::fromJson(const rapidjson::Value &json) {
    BaseClassProperties props = ClientRequest::extractBaseClassProperties(json);
    if (json.HasMember("cellId")) {
        return new SellBuildingRequest(props, json["cellId"].GetString());
    } else {
        throw MonopolyException("Could not find 'cellId' or 'value' in SellBuildingRequest");
    }
}

void SellBuildingRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
    rapidjson::Value cellIdVal(_cellId.c_str(), allocator);
    json.AddMember("cellId", cellIdVal,allocator);
}
