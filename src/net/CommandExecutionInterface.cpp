#include "../../include/CommandExecutionInterface.h"

std::vector<CommandExecutionInterface>& operator<<(std::vector<CommandExecutionInterface>& vec, const CommandExecutionInterface& cmdExec) {
    vec.push_back(cmdExec);
    return vec;
}