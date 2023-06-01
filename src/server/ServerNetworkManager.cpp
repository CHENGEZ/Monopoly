//
// Created by Manuel on 12.02.2021.
//
// The ServerNetworkManager handles all incoming messages and offers functionality to broadcast messages
// to all connected players of a game.

#include "ServerNetworkManager.h"
#include "RequestHandler.h"

// include server address configurations
#include "../common/network/default.conf"
#include "../common/network/responses/RequestResponse.h"

using namespace std;


ServerNetworkManager::ServerNetworkManager() {
    if (_instance == nullptr) {
        _instance = this;
    }
    sockpp::socket_initializer socket_initializer; // Required to initialise sockpp
    this->connect(default_server_host, default_port);   // variables from "default.conf"
}

ServerNetworkManager::~ServerNetworkManager() = default;

void ServerNetworkManager::connect(const std::string &url, const uint16_t port) {
    this->_acc = sockpp::tcp_acceptor(port);

    if (!_acc) {
        std::cerr << "Error creating the acceptor: " << _acc.last_error_str() << std::endl;
        return;
    }

    std::cout << "Awaiting connections on port " << port << "..." << std::endl;
    listenerLoop();    // start endless loop
}

void ServerNetworkManager::listenerLoop() {
    // intentional endless loop
    while (true) {
        sockpp::inet_address peer;

        // Accept a new client connection
        sockpp::tcp_socket sock = _acc.accept(&peer);
        std::cout << "Received a connection request from " << peer << std::endl;

        if (!sock) {
            std::cerr << "Error accepting incoming connection: "
                      << _acc.last_error_str() << std::endl;
        } else {
            _rwLock.lock();
            __playerIdToSocket.emplace(sock.peer_address().to_string(), std::move(sock.clone()));
            _rwLock.unlock();
            // Create a listener thread and transfer the new stream to it.
            // Incoming messages will be passed to handle_incoming_message().
            std::thread listener(
                    readMessage,
                    std::move(sock),
                    handle_incoming_message);

            listener.detach();
        }
    }
}

// Runs in a thread and reads anything coming in on the 'socket'.
// Once a message is fully received, the string is passed on to the 'handle_incoming_message()' function
void ServerNetworkManager::readMessage(sockpp::tcp_socket socket, const std::function<void(const std::string&,
                                                                                           const sockpp::tcp_socket::addr_t &)> &messageHandler) {
    sockpp::socket_initializer sockInit;    // initializes socket framework underneath

    char buffer[512]; // 512 bytes
    ssize_t count = 0;
    ssize_t msg_bytes_read = 0;
    ssize_t msg_length = 0;

    while ((count = socket.read(buffer, sizeof(buffer))) > 0) {
        try {
            int i = 0;
            std::stringstream ss_msg_length;
            while (buffer[i] != ':' && i < count) {
                ss_msg_length << buffer[i];
                i++;
            }
            msg_length = std::stoi(ss_msg_length.str());
            std::cout << "Expecting message of length " << msg_length << std::endl;

            // put everything after the message length declaration into a stringstream
            std::stringstream ss_msg;
            ss_msg.write(&buffer[i+1], count - (i + 1));
            msg_bytes_read = count - (i + 1);

            // read the remaining packages
            while (msg_bytes_read < msg_length && count > 0) {
                count = socket.read(buffer, sizeof(buffer));
                msg_bytes_read += count;
                ss_msg.write(buffer, count);
            }

            if (msg_bytes_read == msg_length) {
                // sanity check that really all bytes got read (possibility that count was <= 0, indicating a read error)
                std::string msg = ss_msg.str();
                messageHandler(msg, socket.peer_address());    // attempt to parse ClientRequest from 'msg'
            } else {
                std::cerr << "Could not read entire message. TCP stream ended before. Difference is " << msg_length - msg_bytes_read << std::endl;
            }
        } catch (std::exception& e) { // Make sure the connection isn't torn down only because of a read error
            std::cerr << "Error while reading message from " << socket.peer_address() << std::endl << e.what() << std::endl;
        }
    }
    if (count <= 0) {
        std::cout << "Read error [" << socket.last_error() << "]: "
                  << socket.last_error_str() << std::endl;
    }

    std::cout << "Closing connection to " << socket.peer_address() << std::endl;
    socket.shutdown();
}


void ServerNetworkManager::handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address) {
    try {
        // try to parse a json from the 'msg'
        rapidjson::Document req_json;
        req_json.Parse(msg.c_str());
        // try to parse a ClientRequest from the json
        ClientRequest* req = ClientRequest::fromJson(req_json);

        // check if this is a connection to a new player
        std::string playerId = req->getPlayerId();
//        cerr << "Player id is: " << playerId << endl;
        _rwLock.lock_shared();
        if (_playerIdToAddress.find(playerId) == _playerIdToAddress.end()) {
            // save connection to this client
            _rwLock.unlock_shared();
            std::cout << "New client with id " << playerId << std::endl;
            _rwLock.lock();
            _playerIdToAddress.emplace(playerId, peer_address.to_string());
            _rwLock.unlock();
        } else {
            _rwLock.unlock_shared();
        }
#ifdef PRINT_NETWORK_MESSAGES
        std::cout << "Received valid request : " << msg << std::endl;
#endif
        // execute client request
        ServerResponse* res = request_handler::handle_request(req);
        delete req;

        // transform response into a json
        rapidjson::Document* resJson = res->toJson();
        delete res;

        // transform json to string
        std::string res_msg = json_utils::toString(resJson);

#ifdef PRINT_NETWORK_MESSAGES
        std::cout << "Sending response : " << res_msg << std::endl;
#endif

        // send response back to client
        sendMessage(res_msg, peer_address.to_string());
        delete resJson;
    } catch (const std::exception& e) {
        std::cerr << "Failed to execute client request. Content was :\n"
                  << msg << std::endl
                  << "Error was " << e.what() << std::endl;
    }
}


void ServerNetworkManager::onPlayerLeft(std::string playerId) {
    _rwLock.lock();
    std::string address = _playerIdToAddress[playerId];
    _playerIdToAddress.erase(playerId);
    __playerIdToSocket.erase(address);
    _rwLock.unlock();
}

ssize_t ServerNetworkManager::sendMessage(const std::string &msg, const std::string& address) {

    std::stringstream ss_msg;
    ss_msg << std::to_string(msg.size()) << ':' << msg; // prepend message length
    return __playerIdToSocket.at(address).write(ss_msg.str());
}

void ServerNetworkManager::broadcast_message(ServerResponse &msg, const std::vector<Player *> &players,
                                               const Player *exclude) {
    rapidjson::Document* msgJson = msg.toJson();  // write to JSON format
    std::string msgString = json_utils::toString(msgJson);   // convert to string

#ifdef PRINT_NETWORK_MESSAGES
    std::cout << "Broadcasting message : " << msgString << std::endl;
#endif

    _rwLock.lock_shared();
    // send object_diff to all requested players
    try {
        for(auto& player : players) {
            if (player != exclude) {
                int nof_bytes_written = sendMessage(msgString, _playerIdToAddress.at(player->getId()));
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Encountered error when sending state update: " << e.what() << std::endl;
    }
    _rwLock.unlock_shared();
    delete msgJson;
}



