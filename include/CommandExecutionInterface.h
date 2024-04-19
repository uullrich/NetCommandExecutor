#ifndef COMMAND_EXECUTION_INTERFACE_H
#define COMMAND_EXECUTION_INTERFACE_H

#include <functional>

#include "../src/util/StringVector.h"

struct Command;

struct CommandExecutionInterface {
    StringVector parts;
    std::function<void(Command &command)> setMethod;
    std::function<void(Command &command)> getMethod;
    std::function<void(Command &command)> setDataMethod;
    std::function<void(Command &command)> getDataMethod;
};
#endif
