#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/use_awaitable.hpp>

#include "Client.h"
#include "CommandReceivedHandler.h"

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

namespace this_coro = boost::asio::this_coro;

class TcpServer {
public:
    TcpServer(unsigned short port, CommandReceivedHandler &commandHandler);

    ClientList getClients();

    void removeClient(ClientPtr client);

private:
    awaitable<void> listener(tcp::acceptor acceptor);

    unsigned short _port;
    ClientList _clients;
    CommandReceivedHandler &_commandHandler;
};

#endif