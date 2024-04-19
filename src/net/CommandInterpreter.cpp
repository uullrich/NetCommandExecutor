#include "../../include/CommandInterpreter.h"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "../../include/Command.h"

Command CommandInterpreter::interpret(std::string &message) {
    Command command{
            .type = CommandType::UNKNOWN
    };
    message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

    StringVector split;
    boost::split(split, message, boost::is_any_of(" "));

    if (split.size() == 2) {
        if (split.front() == "GET") {
            command.type = CommandType::GET;
        } else if (split.front() == "GET-DATA") {
            command.type = CommandType::GET_DATA;
        } else if (split.front() == "SET") {
            command.type = CommandType::SET;
        } else if (split.front() == "SET-DATA") {
            command.type = CommandType::SET_DATA;
        }

        auto uri = split.back();
        split.clear();
        boost::split(split, uri, boost::is_any_of("/"));
        command.parts = split;

        if (command.type == CommandType::SET || command.type == CommandType::SET_DATA) {
            split.clear();
            boost::split(split, command.parts.back(), boost::is_any_of("="));

            command.parts.back() = split.front();
            command.value = split.back();
        }
    }

    return command;
}