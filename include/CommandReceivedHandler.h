#ifndef MESSAGE_RECEIVED_HANDLER_H
#define MESSAGE_RECEIVED_HANDLER_H

#include <iostream>

struct Command;

class CommandReceivedHandler {
public:
    virtual void onCommandReceived(Command &message) = 0;
};

#endif