//
// Created by huyux on 30/4/2023.
//

#ifndef MONOPOLY_BUY_PROPERTY_REQUEST_H
#define MONOPOLY_BUY_PROPERTY_REQUEST_H


#include "ClientRequest.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class BuyPropertyRequest : public ClientRequest {

private:

    std::string _cellId;

    //Private constructor for deserialization
    BuyPropertyRequest(BaseClassProperties, std::string _cellId);

public:
    [[nodiscard]] std::string getCellId() const { return this->_cellId; }

    BuyPropertyRequest(std::string gameId, std::string playerId, std::string cellId);
    virtual void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static BuyPropertyRequest* fromJson(const rapidjson::Value& json);
};


#endif //MONOPOLY_BUY_PROPERTY_REQUEST_H
