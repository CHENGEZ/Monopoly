//
// Created by Manuel on 28.01.2021.
//
// client_request is the base-class for all requests that are being sent from the client to the server.
// It offers a function to deserialize a client_request subclass from a valid json.

#ifndef MONOPOLY_CLIENT_REQUEST_H
#define MONOPOLY_CLIENT_REQUEST_H

#include <string>
#include <unordered_map>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/serializable.h"
#include "../../exceptions/MonopolyException.hpp"
#include "../../serialization/uuid_generator.h"
#include "../../serialization/json_utils.h"

// Identifier for the different request types.
// The RequestType is sent with every client_request to identify the type of client_request
// during deserialization on the server side.
enum RequestType {
    JOIN_LOBBY,
    START_GAME,
    END_TURN,
    BUY_PROPERTY,
    SELL_PROPERTY,
    BUY_BUILDING,
    SELL_BUILDING,
    ROLL_DICE,
    USE_GET_OUT_OF_JAIL_FREE,
};

class ClientRequest : public serializable {
protected:

    struct BaseClassProperties {
        RequestType _type;
        std::string _reqId;
        std::string _playerId;
        std::string _gameId;
    };

    RequestType _type;
    std::string _reqId;
    std::string _playerId;
    std::string _gameId;

    explicit ClientRequest(BaseClassProperties); // base constructor
    static BaseClassProperties createBaseClassProperties(RequestType type, std::string reqId, std::string& playerId, std::string& gameId);
    static BaseClassProperties extractBaseClassProperties(const rapidjson::Value& json);

private:

//    // for deserialization
//    static const std::unordered_map<std::string, RequestType> StringToRequestType;
//    // for serialization
//    static const std::unordered_map<RequestType, std::string> RequestTypeToString;

public:
    //Shifted to public to allow for external access
    // for deserialization
    static const std::unordered_map<std::string, RequestType> StringToRequestType;
    // for serialization
    static const std::unordered_map<RequestType, std::string> RequestTypeToString;

    virtual ~ClientRequest() = default;

    [[nodiscard]] RequestType getType() const { return this->_type; }
    [[nodiscard]] std::string getReqId() const { return this->_reqId; }
    [[nodiscard]] std::string getPlayerId() const { return this->_playerId; }
    [[nodiscard]] std::string getGameId() const { return this->_gameId; }

    // Tries to create the specific client_request from the provided json.
    // Throws exception if parsing fails -> Use only in "try{ }catch()" block
    static ClientRequest* fromJson(const rapidjson::Value& json);

    // Serializes the client_request into a json object that can be sent over the network
    void writeIntoJson(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

    [[nodiscard]] virtual std::string toString() const;
};


#endif //MONOPOLY_CLIENT_REQUEST_H
