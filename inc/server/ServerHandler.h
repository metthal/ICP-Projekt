#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <atomic>
#include <thread>
#include <unordered_map>
#include "server/TcpServer.h"
#include "server/Session.h"
#include "common/Packet.h"

class ServerHandler;
typedef std::unordered_map<uint8_t, void (ServerHandler::*)(SessionPtr, PacketPtr)> HandlerTable;

class ServerHandler
{
public:
    ServerHandler() = delete;
    ServerHandler(const ServerHandler&) = delete;
    ServerHandler(TcpServerPtr& server);

    ~ServerHandler();

    void start();
    void stop();

    void HandleUnknown(SessionPtr session, PacketPtr packet);
    void HandleHandshakeRequest(SessionPtr session, PacketPtr packet);
    void HandleGameListRequest(SessionPtr session, PacketPtr packet);
    void HandleGameJoinRequest(SessionPtr session, PacketPtr packet);

private:
    ServerHandler& operator =(const ServerHandler&);

    void startImpl();

    std::thread _thread;
    std::atomic_bool _running;
    TcpServerPtr _server;
    HandlerTable _handlerTable;
};

#endif // SERVER_HANDLER_H
