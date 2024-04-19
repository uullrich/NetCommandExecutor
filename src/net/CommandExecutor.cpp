#include "../../include/CommandExecutor.h"

#include "../../include/Command.h"
#include "../../include/CommandExecutionInterface.h"

void CommandExecutor::doCommandExecution(Command &command, std::vector<CommandExecutionInterface> &execInterfaces) {
    auto commandPartsCount = command.parts.size();
    for (int i = 0; i < execInterfaces.size(); i++) {
        auto &execInterface = execInterfaces[i];
        bool match = true;
        if (commandPartsCount >= execInterface.parts.size()) {
            for (int j = 0; j < execInterface.parts.size(); j++) {
                if (execInterface.parts[j] != "*" && execInterface.parts[j] != command.parts[j]) {
                    match = false;
                    break;
                }
            }
        }

        if (match) {
            if (execInterface.setMethod && command.type == CommandType::SET) {
                execInterface.setMethod(command);
            } else if (execInterface.getMethod && command.type == CommandType::GET) {
                execInterface.getMethod(command);
            } else if (execInterface.setDataMethod && command.type == CommandType::SET_DATA) {
                execInterface.setDataMethod(command);
            } else if (execInterface.getDataMethod && command.type == CommandType::GET_DATA) {
                execInterface.getDataMethod(command);
            } else {
                //Command not supported
                //ToDo: Log output
            };
            break;
        }
    }
}
