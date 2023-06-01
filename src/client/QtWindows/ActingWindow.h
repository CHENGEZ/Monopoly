//
// Created by Ricky on 2023/5/10.
//

#ifndef MONOPOLY_ACTINGWINDOW_H
#define MONOPOLY_ACTINGWINDOW_H

#include <iostream>


class ActingWindow{

protected:
    bool _needUpdate = false;
    bool _windowClosed = false;
    std::string _recievedMessage;

public:
    virtual void parseResponse(const std::string message)=0;
};


#endif //MONOPOLY_ACTINGWINDOW_H
