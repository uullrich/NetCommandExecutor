#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "CommandReceivedHandler.h"

struct Command;
struct CommandExecutionInterface;

class CommandHandler : public CommandReceivedHandler {
public:
    explicit CommandHandler(std::vector<CommandExecutionInterface> &commandExecutionInterface);

    void onCommandReceived(Command &command) override;

private:
    std::vector<CommandExecutionInterface> &_commandExecutionInterface;
};

#endif