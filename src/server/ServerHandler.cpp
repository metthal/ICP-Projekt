#include <chrono>
#include "server/ServerHandler.h"
#include "common/Opcode.h"
#include "common/Log.h"

ServerHandler::ServerHandler(TcpServerPtr& server) : _thread(), _running(false), _server(server)
{
    for (uint8_t opcode = FIRST_OPCODE; opcode < MAX_OPCODES; ++opcode)
        _handlerTable[opcode] = &ServerHandler::HandleUnknown;

    _handlerTable[CMSG_HANDSHAKE_REQUEST] = &ServerHandler::HandleHandshakeRequest;
    _handlerTable[CMSG_GAME_LIST_REQUEST] = &ServerHandler::HandleGameListRequest;
    _handlerTable[CMSG_GAME_JOIN_REQUEST] = &ServerHandler::HandleGameJoinRequest;
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::start()
{
    _running = true;
    _thread = std::thread(&ServerHandler::startImpl, this);
}

void ServerHandler::stop()
{
    _running = false;
    _thread.join();
}

void ServerHandler::startImpl()
{
    while (_running)
    {
        SessionList sessions = _server->getSessions();
        for (SessionPtr& session : sessions)
        {
            while (PacketPtr packet = session->getReceivedPacket())
                (this->*_handlerTable[packet->getOpcode()])(session, packet);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void ServerHandler::HandleUnknown(SessionPtr, PacketPtr)
{
    sLog.out("HandleUnknown");
}

void ServerHandler::HandleHandshakeRequest(SessionPtr session, PacketPtr)
{
    PacketPtr response = PacketPtr(new Packet(SMSG_HANDSHAKE_RESPONSE, 1));
    *response << true;
    session->send(response);
}

void ServerHandler::HandleGameListRequest(SessionPtr, PacketPtr)
{
    sLog.out("HandleGameListRequest");
}

void ServerHandler::HandleGameJoinRequest(SessionPtr, PacketPtr)
{
    sLog.out("HandleGameJoinRequest");
}
