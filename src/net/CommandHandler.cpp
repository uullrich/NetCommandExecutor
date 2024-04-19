#include "../../include/CommandHandler.h"

#include "../../include/Command.h"
#include "../../include/CommandExecutor.h"

CommandHandler::CommandHandler(std::vector<CommandExecutionInterface> &commandExecutionInterface)
        : _commandExecutionInterface(commandExecutionInterface) {}

void CommandHandler::onCommandReceived(Command &command) {
    CommandExecutor::doCommandExecution(command, _commandExecutionInterface);
}