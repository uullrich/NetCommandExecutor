#ifndef COMMAND_EXECUTION_INTERFACE_H
#define COMMAND_EXECUTION_INTERFACE_H

#include <iostream>
#include <functional>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "../src/util/StringVector.h"

struct Command;

typedef StringVector Endpoint;

enum class NO_RESPONSE {
    GET,
    GET_DATA,
    SET,
    SET_DATA,
};

auto noCommandHandler = [](Command &command) {};

struct CommandExecutionInterface {
    Endpoint parts;
    std::function<void(Command &command)> setMethod;
    std::function<void(Command &command)> getMethod;
    std::function<void(Command &command)> setDataMethod;
    std::function<void(Command &command)> getDataMethod;

    explicit CommandExecutionInterface(void * instance) : _instance(instance) {}

    CommandExecutionInterface(StringVector parts,
                              std::function<void(Command &)> setMethod,
                              std::function<void(Command &)> getMethod,
                              std::function<void(Command &)> setDataMethod,
                              std::function<void(Command &)> getDataMethod)
            : parts(std::move(parts)),
              setMethod(std::move(setMethod)),
              getMethod(std::move(getMethod)),
              setDataMethod(std::move(setDataMethod)),
              getDataMethod(std::move(getDataMethod)),
              _instance(nullptr) {}

    //Fluent interface for setting parts
    CommandExecutionInterface &operator<<(const StringVector &p) {
        parts = p;
        return *this;
    }

    //Helper function to convert member function pointer to lambda
    template<typename T>
    std::function<void(Command &)> memberFunctionToLambda(void (T::*memFunc)(Command &)) {
        auto *inst = static_cast<T *>(_instance);
        return [memFunc, inst](Command &cmd) { (inst->*memFunc)(cmd); };
    }

    //Overload for setting member function pointers
    template<typename T>
    CommandExecutionInterface &operator<<(void (T::*memFunc)(Command &)) {
        if (!_instance) {
            throw std::logic_error("Instance pointer is not set.");
        }

        if (!setMethod) {
            setMethod = memberFunctionToLambda(memFunc);
        } else if (!getMethod) {
            getMethod = memberFunctionToLambda(memFunc);
        } else if (!setDataMethod) {
            setDataMethod = memberFunctionToLambda(memFunc);
        } else if (!getDataMethod) {
            getDataMethod = memberFunctionToLambda(memFunc);
        } else {
            throw std::runtime_error("All method slots are already filled.");
        }
        return *this;
    }

    //Fluent interface for setting no response handlers
    CommandExecutionInterface &operator<<(NO_RESPONSE noResponse) {

        switch (noResponse) {
            case NO_RESPONSE::SET:
                setMethod = noCommandHandler;
                break;
            case NO_RESPONSE::GET:
                getMethod = noCommandHandler;
                break;
            case NO_RESPONSE::SET_DATA:
                setDataMethod = noCommandHandler;
                break;
            case NO_RESPONSE::GET_DATA:
                getDataMethod = noCommandHandler;
                break;
            default:
                throw std::runtime_error("All method slots are already filled.");
        }

        return *this;
    }

    friend std::vector<CommandExecutionInterface> &
    operator<<(std::vector<CommandExecutionInterface> &vec, const CommandExecutionInterface &cmdExec);

private:
    void *_instance;
};

#endif
