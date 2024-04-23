#include "../../include/TcpServer.h"

#include <iostream>

#include "../../include/CommandReceivedHandler.h"
#include "../../include/Client.h"

TcpServer::TcpServer(unsigned short port, CommandReceivedHandler &commandHandler) : _port(port),
                                                                                    _commandHandler(commandHandler) {
    std::cout << "Creating TCP server on port " << port << std::endl;
    try {
        boost::asio::io_context io_context(1);

        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            std::cout << "Stopping TCP server" << std::endl;
            io_context.stop();
        });

        co_spawn(io_context, listener(tcp::acceptor(io_context, {tcp::v4(), _port})), detached);

        io_context.run();
    }
    catch (std::exception &e) {
        std::printf("Exception: %s\n", e.what());
    }
}

awaitable<void> TcpServer::listener(tcp::acceptor acceptor) {
    while(true) {
        auto socket = co_await acceptor.async_accept(use_awaitable);
        auto client = std::make_shared<Client>(std::move(socket), _commandHandler, *this);
        client->initHandlers();

        _clients.emplace_back(client);
    }
}

ClientList TcpServer::getClients() {
    return _clients;
}

void TcpServer::removeClient(const ClientPtr client) {
    auto it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
    }
}