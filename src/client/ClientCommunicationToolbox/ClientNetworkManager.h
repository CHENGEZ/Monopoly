//
// Created by Ricky on 2023/5/9.
//

#ifndef MONOPOLY_CLIENTNETWORKMANAGER_H
#define MONOPOLY_CLIENTNETWORKMANAGER_H

#include <string>
#include "sockpp/socket.h"
#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "../QtWindows/ActingWindow.h"
#include "ResponseListenerThread.h"
#include "../../common/network/requests/ClientRequest.h"
#include <QObject>
//#include "../../sockpp/"


class ClientNetworkManager{
public:

    static void init(const std::string& h, const uint16_t port);

    static bool connect(const std::string& host, const uint16_t port);
    static void sendRequest(const ClientRequest& request);

    static void setActingWindow(ActingWindow* actingWindowToSet);
    static void setTCPConnector(sockpp::tcp_connector* connectionToSet){_connection = connectionToSet;}

    static bool checkConnected();


private:

    static sockpp::tcp_connector* _connection;

    static ActingWindow* _actingWindow;
    static ResponseListenerThread* _listenerThread;
    static bool _connectSuccess;
    static bool _failedToConnect;

};


#endif //MONOPOLY_CLIENTNETWORKMANAGER_H
