#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <deque>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>

#include "../src/util/Buffer.h"

using boost::asio::awaitable;
using boost::asio::ip::tcp;
using boost::asio::const_buffer;
using boost::asio::steady_timer;

class CommandReceivedHandler;

class TcpServer;

struct Command;

constexpr int receiveBufferSize = 1024;

class Client : public std::enable_shared_from_this<Client> {
public:
    Client(tcp::socket socket, CommandReceivedHandler &commandHandler, TcpServer &tcpServer);

    void initHandlers();

    const std::string &getIp();

    void send(const std::vector<unsigned char> &data);

    void send(const std::string &data);

    void disconnect();

private:
    awaitable<void> onReceiveHandler();

    awaitable<void> onSendHandler();

    tcp::socket _socket;
    std::string _ip;

    CommandReceivedHandler &_commandHandler;
    TcpServer &_tcpServer;

    steady_timer _timer;
    SendBuffer _sendBuffer;
    ReceiveBuffer _receiveBuffer;
};

typedef std::shared_ptr<Client> ClientPtr;
typedef std::vector<ClientPtr> ClientList;
#endif