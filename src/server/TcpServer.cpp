#include <boost/bind.hpp>
#include "server/TcpServer.h"

TcpServer::TcpServer(uint16_t port) : _thread(), _ioService(), _localEndpoint(boost::asio::ip::tcp::v4(), port), _acceptor(_ioService, _localEndpoint)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    _thread = std::thread(&TcpServer::startImpl, this);
}

void TcpServer::stop()
{
    _ioService.stop();
    _thread.join();
}

void TcpServer::startImpl()
{
    startAccept();
    _ioService.run();
}

void TcpServer::startAccept()
{
    SessionPtr newSession = SessionPtr(new Session(_ioService));

    _acceptor.async_accept(newSession->getSocket(), boost::bind(&TcpServer::handleAccept, this, newSession, boost::asio::placeholders::error));
}

void TcpServer::handleAccept(SessionPtr session, const boost::system::error_code& error)
{
    if (error)
        return;

    std::unique_lock<std::mutex> lock(_sessionsMutex);

    _sessions.push_back(session);
    session->start();

    startAccept();
}

SessionList TcpServer::getSessions()
{
    std::unique_lock<std::mutex> lock(_sessionsMutex);
    return _sessions;
}
