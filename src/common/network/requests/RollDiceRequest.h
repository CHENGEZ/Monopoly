//
// Created by huyux on 30/4/2023.
//

#ifndef MONOPOLY_ROLL_DICE_REQUEST_H
#define MONOPOLY_ROLL_DICE_REQUEST_H

#include <string>
#include "ClientRequest.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class RollDiceRequest : public ClientRequest{

private:

    /*
     * Private constructor for deserialization
     */
    explicit RollDiceRequest(BaseClassProperties);

public:
    RollDiceRequest(std::string gameId, std::string playerId);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static RollDiceRequest* fromJson(const rapidjson::Value& json);
};



#endif //MONOPOLY_ROLL_DICE_REQUEST_H
