//
// Created by Ricky on 2023/5/9.
//

#include <sockpp/exception.h>
#include <sockpp/socket.h>
#include "../HelperFunctions.cpp"
#include "ClientNetworkManager.h"
#include "ResponseListenerThread.h"


#include <QThread>
#include <iostream>

using namespace std;

// initialize static members


sockpp::tcp_connector* ClientNetworkManager::_connection = nullptr;

bool ClientNetworkManager::_connectSuccess = false;
bool ClientNetworkManager::_failedToConnect = false;

ActingWindow* ClientNetworkManager::_actingWindow = nullptr;
ResponseListenerThread* ClientNetworkManager::_listenerThread = nullptr;

QMetaObject::Connection currConnection;


void ClientNetworkManager::init(const std::string& host, const uint16_t port) {

    // init sockpp framework
    sockpp::socket_initializer sockInit;

    cout << host << endl;

    ClientNetworkManager::_connectSuccess = false;
    ClientNetworkManager::_failedToConnect = false;

    // delete exiting conection and create new one
    if(ClientNetworkManager::_connection != nullptr){
        ClientNetworkManager::_connection->shutdown();
        delete ClientNetworkManager::_connection;
    }

    ClientNetworkManager::_connection = new sockpp::tcp_connector();

    //try to connect the server
    if(ClientNetworkManager::connect(host, port)){
        showMessage("Connected to " + host + ":" + std::to_string(port));
        ClientNetworkManager::_connectSuccess = true;

        ActingWindow* currActingWindow = ClientNetworkManager::_actingWindow;

        if (ClientNetworkManager::_listenerThread != nullptr){
            cout << "Finding standing listener thread, shut it down." << endl;
            delete _listenerThread;
        }

        ClientNetworkManager::_listenerThread = new ResponseListenerThread(nullptr, ClientNetworkManager::_connection);
        currConnection = QObject::connect(_listenerThread,
                         &ResponseListenerThread::recievedVaildFeedback,
                         [currActingWindow](const std::string& feedback){currActingWindow->parseResponse(feedback);}
        );
        ClientNetworkManager::_listenerThread->start();
    }else{
        ClientNetworkManager::_failedToConnect = true;
        showError("Not connected");
    }
}

bool ClientNetworkManager::connect(const std::string& host, const uint16_t port) {

    // create sockpp address and catch any errors
    sockpp::inet_address address;
    try{
        address = sockpp::inet_address(host, port);
    } catch (const sockpp::getaddrinfo_error& e){
        showError("Failed to connect to server " + address.to_string());
        return false;
    }

    if (!ClientNetworkManager::_connection->connect(address)){
        showError("Failed to connect to server " + address.to_string(), "Connection error");
        return false;
    }
    return true;
}

bool ClientNetworkManager::checkConnected() {
    if (ClientNetworkManager::_connectSuccess && ClientNetworkManager::_connection->is_connected()){
        return true;
    }
    return false;
}


// Current version is purely for testing
void ClientNetworkManager::sendRequest(const ClientRequest &request) {
    int connectionCheckCounter = 0;

    while(!ClientNetworkManager::_connectSuccess
          && !ClientNetworkManager::_failedToConnect
          && connectionCheckCounter < 200){
        QThread::msleep(25);
        connectionCheckCounter ++;
    }

    // do not continue if failed to connect to server
    if(ClientNetworkManager::_failedToConnect) {
        return;
    }

    if(ClientNetworkManager::_connectSuccess && ClientNetworkManager::_connection->is_connected()){
        rapidjson::Document* jsonDocument = request.toJson();
        std::string message = json_utils::toString(jsonDocument);
        delete jsonDocument;

        std::stringstream messageStream;
        messageStream << std::to_string(message.size()) << ':' << message;
        message = messageStream.str();

        cout << "Sending request: " << message << endl;

        ssize_t bytesSent = ClientNetworkManager::_connection->write(message);

        if (bytesSent != ssize_t(message.length())) {
            showError("Error writing to the TCP stream: " + ClientNetworkManager::_connection->last_error_str(),
                      "Network error");
        }
    }else{
        showError("Lost connection to server", "Network error");
    }
}

void ClientNetworkManager::setActingWindow(ActingWindow *actingWindowToSet) {
    if(ClientNetworkManager::_listenerThread == nullptr){
        cout << "initial setup of acting window" << endl;
        ClientNetworkManager::_actingWindow = actingWindowToSet;
    }else{
        cout << "in-program setup of acting window, break old connection and setup new windows." << endl;
        QObject::disconnect(currConnection);
        ClientNetworkManager::_actingWindow = actingWindowToSet;
        ActingWindow* currActingWindow = ClientNetworkManager::_actingWindow;
        ClientNetworkManager::_actingWindow = actingWindowToSet;
        currConnection = QObject::connect(_listenerThread,
                         &ResponseListenerThread::recievedVaildFeedback,
                         [currActingWindow](const std::string& feedback){currActingWindow->parseResponse(feedback);}
        );
    }
}



