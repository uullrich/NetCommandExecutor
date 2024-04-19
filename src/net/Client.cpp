#include "../../include/Client.h"

#include <iostream>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio.hpp>
#include <utility>

#include "../../include/CommandReceivedHandler.h"
#include "../../include/Command.h"
#include "../../include/CommandInterpreter.h"
#include "../../include/TcpServer.h"

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::redirect_error;
using boost::asio::ip::tcp;

Client::Client(tcp::socket socket, CommandReceivedHandler &commandHandler, TcpServer &tcpServer)
        : _socket(std::move(socket)), _commandHandler(commandHandler), _tcpServer(tcpServer),
          _timer(_socket.get_executor()), _receiveBuffer(receiveBufferSize) {
    std::cout << "Created Client with socketIsOpen: " << std::endl;
    _timer.expires_at(std::chrono::steady_clock::time_point::max());
    _ip = _socket.remote_endpoint().address().to_string();
    _receiveBuffer.clear();
}

void Client::send(const std::string &data) {
    _sendBuffer.emplace_back(data.begin(), data.end());
    _timer.cancel_one();
}

void Client::send(const std::vector<unsigned char> &data) {
    _sendBuffer.emplace_back(data);
    _timer.cancel_one();
}

void Client::disconnect() {
    _socket.close();
    _timer.cancel();
    _tcpServer.removeClient(shared_from_this());
}

void Client::initHandlers() {
    co_spawn(
            _socket.get_executor(),
            [self = shared_from_this()] { return self->onReceiveHandler(); },
            detached);

    co_spawn(
            _socket.get_executor(),
            [self = shared_from_this()] { return self->onSendHandler(); },
            detached);
}

const std::string &Client::getIp() {
    return _ip;
}

awaitable<void> Client::onReceiveHandler() {
    try {

        while (true) {
            std::size_t delimiterPosition = co_await boost::asio::async_read_until(_socket,
                                                                                   boost::asio::dynamic_buffer(
                                                                                           _receiveBuffer),
                                                                                   "\n",
                                                                                   use_awaitable);

            std::string receivedMessage(_receiveBuffer.begin(),
                                        _receiveBuffer.begin() + static_cast<long>(delimiterPosition));

            _receiveBuffer.erase(_receiveBuffer.begin(), _receiveBuffer.begin() + static_cast<long>(delimiterPosition));

            auto command = CommandInterpreter::interpret(receivedMessage);
            command.client = shared_from_this();

            if (command.type == CommandType::SET_DATA) {
                co_await boost::asio::async_read(_socket,
                                                 boost::asio::dynamic_buffer(command.data),
                                                 boost::asio::transfer_exactly(
                                                         std::stoi(command.value)),
                                                 use_awaitable);
            }

            if (command.type != CommandType::UNKNOWN) {
                _commandHandler.onCommandReceived(command);
            }
        }
    }
    catch (std::exception &exception) {
        std::cout << "Error: " << exception.what() << std::endl;
        disconnect();
    }
}

awaitable<void> Client::onSendHandler() {
    try {
        while (_socket.is_open()) {
            if (_sendBuffer.empty()) {
                boost::system::error_code ec;
                co_await _timer.async_wait(redirect_error(use_awaitable, ec));
            } else {
                co_await boost::asio::async_write(_socket,
                                                  boost::asio::buffer(_sendBuffer.front()), use_awaitable);
                _sendBuffer.pop_front();
            }
        }
    }
    catch (std::exception &) {
        disconnect();
    }
}