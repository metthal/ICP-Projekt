/*
 * @file TcpServer.h
 *
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
 * @author Marek Milkovič (xmilko01)
 * @author Ivan Ševčík (xsevci50)
 */

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

    /**
     * Start the server in its own thread.
     */
    void start();

    /**
     * Stops the server
     */
    void stop();

    /**
     * Waits until the server stop
     */
    void wait();

    /**
     * Returns the list of active sessions.
     * @return List of sessions.
     */
    SessionList getSessions();

    /**
     * Removes some sessions from the active sessions and disconnects them.
     * @param sessionList List of session to remove.
     */
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
