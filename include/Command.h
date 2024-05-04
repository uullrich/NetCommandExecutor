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

    void sendValue(const std::string &value);

    void sendData(const std::vector<unsigned char> &data);

    void sendData(const std::string & data);

    void sendOk();

    void sendError(const std::string &message);
};

#endif
