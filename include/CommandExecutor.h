#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <vector>

struct CommandExecutionInterface;
struct Command;

class CommandExecutor {
public:
    static void doCommandExecution(Command &command, std::vector<CommandExecutionInterface> &execInterfaces);
};

#endif
