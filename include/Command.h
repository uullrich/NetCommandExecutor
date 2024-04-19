#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

#include "Client.h"
#include "../src/util/StringVector.h"
#include "../src/util/Buffer.h"

enum class CommandType {
    UNKNOWN,
    GET,
    GET_DATA,
    SET,
    SET_DATA,
};

struct Command {
    CommandType type;
    StringVector parts;
    std::string value;
    ReceiveBuffer data;
    ClientPtr client;
};

#endif
