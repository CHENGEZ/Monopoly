//
// Created by Manuel on 28.01.2021.
//

#include "BuyBuildingRequest.h"

// Public constructor
BuyBuildingRequest::BuyBuildingRequest(std::string gameId, std::string player_id, std::string cellId)
        : ClientRequest(
        ClientRequest::createBaseClassProperties(RequestType::BUY_BUILDING, uuid_generator::generate_uuid_v4(),
                                                 player_id, gameId) ),
          _cellId(cellId)
{ }

// private constructor for deserialization
BuyBuildingRequest::BuyBuildingRequest(ClientRequest::BaseClassProperties props, std::string cellId) :
        ClientRequest(props),
        _cellId(cellId)
{ }

BuyBuildingRequest* BuyBuildingRequest::fromJson(const rapidjson::Value &json) {
    BaseClassProperties props = ClientRequest::extractBaseClassProperties(json);
    if (json.HasMember("cellId")) {
        return new BuyBuildingRequest(props, json["cellId"].GetString());
    } else {
        throw MonopolyException("Could not find 'cellId' or 'value' in BuyBuildingRequest");
    }
}

void BuyBuildingRequest::writeIntoJson(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<> &allocator) const {
    ClientRequest::writeIntoJson(json, allocator);
    rapidjson::Value cellIdVal(_cellId.c_str(), allocator);
    json.AddMember("cellId", cellIdVal,allocator);
}
