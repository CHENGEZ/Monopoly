//
// Created by huyux on 30/4/2023.
//

#ifndef MONOPOLY_END_TURN_REQUEST_H
#define MONOPOLY_END_TURN_REQUEST_H

#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class EndTurnRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit EndTurnRequest(BaseClassProperties);

public:
    EndTurnRequest(std::string gameId, std::string playerId);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static EndTurnRequest* fromJson(const rapidjson::Value& json);
};



#endif //MONOPOLY_END_TURN_REQUEST_H
