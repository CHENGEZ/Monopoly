#include <sstream>
#include "ResponseListenerThread.h"
#include "../HelperFunctions.cpp"


void ResponseListenerThread::run() {
    try {
        char buffer[512]; // 512 bytes
        ssize_t count = 0;

        while ((count = this->_connection->read(buffer, sizeof(buffer))) > 0) {
            try {
                int pos = 0;

                // extract length of message in bytes (which is sent at the start of the message, and is separated by a ":")
                std::stringstream messageLengthStream;
                while (buffer[pos] != ':' && pos < count) {
                    messageLengthStream << buffer[pos];
                    pos++;
                }
                ssize_t messageLength = std::stoi(messageLengthStream.str());

                // initialize a stream for the message
                std::stringstream messageStream;

                // copy everything following the message length declaration into a stringstream
                messageStream.write(&buffer[pos + 1], count - (pos + 1));
                ssize_t bytesReadSoFar = count - (pos + 1);

                // read remaining packages until full message length is reached
                while (bytesReadSoFar < messageLength && count != 0) {
                    count = this->_connection->read(buffer, sizeof(buffer));
                    messageStream.write(buffer, count);
                    bytesReadSoFar += count;
                }

                // process message (if we've received entire message)
                if (bytesReadSoFar == messageLength) {
                    std::string message = messageStream.str();
//                    GameController::getMainThreadEventHandler()->CallAfter([message]{
//                        ClientNetworkManager::parseResponse(message);
//                    });
                    emit recievedVaildFeedback(message);

                } else {
                    this->outputError("Network error", "Could not read entire message. TCP stream ended early. Difference is " + std::to_string(messageLength - bytesReadSoFar) + " bytes");
                }

            } catch (std::exception& e) {
                // Make sure the connection isn't terminated only because of a read error
                this->outputError("Network error", "Error while reading message: " + (std::string) e.what());
            }
        }

        if (count <= 0) {
            this->outputError("Network error", "Read error [" + std::to_string(this->_connection->last_error()) + "]: " + this->_connection->last_error_str());
        }

    } catch(const std::exception& e) {
        this->outputError("Network error", "Error in listener thread: " + (std::string) e.what());
    }

    this->_connection->shutdown();
}

ResponseListenerThread::~ResponseListenerThread() {

}

ResponseListenerThread::ResponseListenerThread(QObject *parent, sockpp::tcp_connector *connection):
    QThread(parent), _connection(connection) {}

void ResponseListenerThread::outputError(std::string title, std::string message) {
    showError(message, title);
}


