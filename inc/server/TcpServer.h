#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <cstdint>
#include <thread>
#include <list>
#include <mutex>
#include <memory>
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

    SessionList getSessions();
    void removeSessions(SessionList& sessionList);

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

typedef std::shared_ptr<TcpServer> TcpServerPtr;

#endif // TCP_SERVER_H
