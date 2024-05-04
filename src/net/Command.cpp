#include "../../include/Command.h"

#include <iostream>
#include <boost/algorithm/string/join.hpp>

void Command::sendValue(const std::string &value) {
    std::stringstream responseStream;
    switch (type) {
        case CommandType::SET:
        case CommandType::SET_DATA:
        case CommandType::GET_DATA:
        case CommandType::UNKNOWN:
            std::cout << "Unsupported operation for command type" << std::endl;
            return;
        case CommandType::GET:
            responseStream << "GET-RESPONSE";
            break;
    }
    responseStream << " " << boost::algorithm::join(parts, "/") << "=" << value << std::endl;
    client->send(responseStream.str());
}

void Command::sendData(const std::vector<unsigned char> &data) {
    std::stringstream responseStream;
    switch (type) {
        case CommandType::SET:
        case CommandType::SET_DATA:
        case CommandType::GET:
        case CommandType::UNKNOWN:
            std::cout << "Unsupported operation for command type" << std::endl;
            return;
        case CommandType::GET_DATA:
            responseStream << "GET-DATA-RESPONSE";
            break;
    }
    responseStream << " " << boost::algorithm::join(parts, "/") << "=" << data.size() << std::endl;
    client->send(responseStream.str());
    client->send(data);
}

void Command::sendOk() {
    std::stringstream responseStream;
    switch (type) {
        case CommandType::GET:
        case CommandType::GET_DATA:
        case CommandType::UNKNOWN:
            std::cout << "Unsupported operation for command type" << std::endl;
            return;
        case CommandType::SET:
            responseStream << "SET-RESPONSE";
            break;
        case CommandType::SET_DATA:
            responseStream << "SET-DATA-RESPONSE";
            break;
    }
    responseStream << " " << boost::algorithm::join(parts, "/") << "=OK" << std::endl;
    client->send(responseStream.str());
}

void Command::sendError(const std::string &message){
    std::vector<unsigned char> rawMessage(message.begin(), message.end());
    std::stringstream responseStream;
    responseStream << "ERROR-RESPONSE " << boost::algorithm::join(parts, "/") << "=" << rawMessage.size() << std::endl;
    client->send(responseStream.str());
    client->send(rawMessage);
}