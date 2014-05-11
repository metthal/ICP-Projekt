/*
 * @file ServerHandler.h
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

#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <atomic>
#include <thread>
#include <unordered_map>
#include "server/TcpServer.h"
#include "server/Session.h"
#include "common/Packet.h"

#define SERVER_GAME_SPEED       16

class ServerHandler;
typedef std::unordered_map<uint8_t, void (ServerHandler::*)(SessionPtr, PacketPtr)> HandlerTable;

class ServerHandler
{
public:
    ServerHandler() = delete;
    ServerHandler(const ServerHandler&) = delete;
    ServerHandler(TcpServerPtr& server);

    ~ServerHandler();

    /**
     * Starts the handler in its own thread.
     */
    void start();

    /**
     * Stops the handler.
     */
    void stop();

protected:
    void HandleUnknown(SessionPtr session, PacketPtr packet);
    void HandleHandshakeRequest(SessionPtr session, PacketPtr packet);
    void HandleGameListRequest(SessionPtr session, PacketPtr packet);
    void HandleGameJoinRequest(SessionPtr session, PacketPtr packet);
    void HandleMapListRequest(SessionPtr session, PacketPtr packet);
    void HandleGameCreateRequest(SessionPtr session, PacketPtr packet);
    void HandlePerformActionRequest(SessionPtr session, PacketPtr packet);
    void HandlePlayerLeftGame(SessionPtr session, PacketPtr packet);

private:
    ServerHandler& operator =(const ServerHandler&);

    void startImpl();

    std::thread _thread;
    std::atomic_bool _running;
    TcpServerPtr _server;
    HandlerTable _handlerTable;
};

#endif // SERVER_HANDLER_H
