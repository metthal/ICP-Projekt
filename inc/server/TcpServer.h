#ifndef TCP_SERVICE_H
#define TCP_SERVICE_H

#include <cstdint>
#include <thread>
#include <list>
#include <mutex>
#include <boost/asio.hpp>
#include "server/Session.h"

typedef std::list<SessionPtr> SessionList;

class TcpServer
{
public:
    TcpServer() = delete;
    TcpServer(const TcpServer&) = delete;
    TcpServer(uint16_t port);

    ~TcpServer();

    void start();
    void stop();

    std::list<SessionPtr> getSessions();

private:
    TcpServer& operator =(const TcpServer&);

    void startImpl();
    void startAccept();
    void handleAccept(SessionPtr session, const boost::system::error_code& error);

    std::thread _thread;
    boost::asio::io_service _ioService;
    boost::asio::ip::tcp::endpoint _localEndpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    SessionList _sessions;
    std::mutex _sessionsMutex;
};

#endif // TCP_SERVICE_H
