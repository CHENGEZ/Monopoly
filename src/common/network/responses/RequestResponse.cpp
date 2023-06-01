//
// Created by Manuel on 15.02.2021.
//

#include "RequestResponse.h"
#include "../../serialization/json_utils.h"
#include "../../exceptions/MonopolyException.hpp"

#ifdef MONOPOLY_CLIENT
#include <QMessageBox>
#endif



RequestResponse::RequestResponse(ServerResponse::BaseClassProperties props, std::string reqId, bool success, rapidjson::Value* stateJson, std::string &err) :
    ServerResponse(props),
    _reqId(reqId),
    _stateJson(stateJson),
    _success(success),
    _err(err)
{ }

RequestResponse::RequestResponse(std::string gameId, std::string reqId, bool success, rapidjson::Value* stateJson, std::string err):
    ServerResponse(ServerResponse::createBaseClassProperties(ResponseType::REQ_RESPONSE, gameId)),
    _reqId(reqId),
    _stateJson(stateJson),
    _success(success),
    _err(err)
{ }


RequestResponse::~RequestResponse() {
    if (_stateJson != nullptr) {
        delete _stateJson;
        _stateJson = nullptr;
    }
}

void RequestResponse::writeIntoJson(rapidjson::Value &json,
                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    ServerResponse::writeIntoJson(json, allocator);

    rapidjson::Value errval(_err.c_str(), allocator);
    json.AddMember("err", errval, allocator);

    rapidjson::Value reqIdVal(_reqId.c_str(), allocator);
    json.AddMember("reqId", reqIdVal, allocator);

    json.AddMember("success", _success, allocator);

    if (_stateJson != nullptr) {
        json.AddMember("stateJson", *_stateJson, allocator);
    }
}


RequestResponse *RequestResponse::fromJson(const rapidjson::Value& json) {
    if (json.HasMember("err") && json.HasMember("success")) {
        std::string err = json["err"].GetString();

        rapidjson::Value* stateJson = nullptr;
        if (json.HasMember("stateJson")) {
            stateJson = json_utils::cloneValue(json["stateJson"].GetObject());
        }
        return new RequestResponse(
                ServerResponse::extractBaseClassProperties(json),
                json["reqId"].GetString(),
                json["success"].GetBool(),
                stateJson,
                err);
    } else {
        throw MonopolyException("Could not parse RequestResponse from json. err or success is missing.");
    }
}

#ifdef MONOPOLY_CLIENT

Game * RequestResponse::Process() const {
    if(_success){
        if(this->_stateJson != nullptr){
            Game* game = Game::fromJson(*_stateJson);
            return game;
        }else{
            QMessageBox::information(nullptr,"Network Error", "Expected a state as JSON inside the request_response. But there was none.");
            return nullptr;
        }
    }else{
        QMessageBox::information(nullptr, "Error", QString::fromStdString(_err));
        return nullptr;
    }
}

#endif
