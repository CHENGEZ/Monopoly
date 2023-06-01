//
// Created by Manuel on 08.02.2022.
//

#ifndef MONOPOLY_REQUEST_HANDLER_H
#define MONOPOLY_REQUEST_HANDLER_H

#include "../common/network/responses/ServerResponse.h"
#include "../common/network/requests/ClientRequest.h"
#include "../common/network/responses/RequestResponse.h"

class request_handler {
public:
    static RequestResponse* handle_request(const ClientRequest* const req);
};
#endif //MONOPOLY_REQUEST_HANDLER_H
