/*
* Project name:
* Bludiste 2014
*
* Description:
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
*
* Project's GitHub repository:
* https://github.com/metthal/ICP-Projekt
*
* Team:
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

#include <boost/bind.hpp>
#include "server/TcpServer.h"
#include "common/Log.h"

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

    std::lock_guard<std::mutex> lock(_sessionsMutex);

    session->start();
    _sessions.push_back(session);

    startAccept();
}

SessionList TcpServer::getSessions()
{
    std::lock_guard<std::mutex> lock(_sessionsMutex);
    return _sessions;
}

void TcpServer::removeSessions(SessionList& sessionList)
{
    std::lock_guard<std::mutex> lock(_sessionsMutex);

    for (SessionPtr& session : sessionList)
    {
        sLog.out("Session ", *session, " dropped");
        _sessions.remove(session);
    }
}
