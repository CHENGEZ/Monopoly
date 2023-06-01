//
// Created by Ricky on 2023/5/13.
//

#include "ConnectionThread.h"
#include "ClientNetworkManager.h"
#include "../HelperFunctions.cpp"

void ConnectionThread::run() {
    ClientNetworkManager::init(this->_host, this->_port);

    if(ClientNetworkManager::checkConnected()){
        emit connected(true);
    }else{
        emit connected(false);
    }

}
