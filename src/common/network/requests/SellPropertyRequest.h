//
// Created by huyux on 30/4/2023.
//

#ifndef MONOPOLY_SELL_PROPERTY_REQUEST_H
#define MONOPOLY_SELL_PROPERTY_REQUEST_H


#include "ClientRequest.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class SellPropertyRequest : public ClientRequest {

private:

    std::string _cellId;

    //Private constructor for deserialization
    SellPropertyRequest(BaseClassProperties, std::string _cellId);

public:
    [[nodiscard]] std::string getCellId() const { return this->_cellId; }

    SellPropertyRequest(std::string gameId, std::string playerId, std::string cellId);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static SellPropertyRequest* fromJson(const rapidjson::Value& json);
};


#endif //MONOPOLY_SELL_PROPERTY_REQUEST_H