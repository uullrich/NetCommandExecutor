#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>

struct Command;

class CommandInterpreter {
public:
    static Command interpret(std::string &message);
};

#endif
