#include <chrono>
#include "server/ServerHandler.h"
#include "common/Opcode.h"
#include "common/Log.h"

ServerHandler::ServerHandler(TcpServerPtr& server) : _thread(), _running(false), _server(server)
{
    for (uint8_t opcode = FIRST_OPCODE; opcode < MAX_OPCODES; ++opcode)
        _handlerTable[opcode] = &ServerHandler::HandleUnknown;

    _handlerTable[CMSG_HANDSHAKE_REQUEST]   = &ServerHandler::HandleHandshakeRequest;
    _handlerTable[CMSG_GAME_LIST_REQUEST]   = &ServerHandler::HandleGameListRequest;
    _handlerTable[CMSG_GAME_JOIN_REQUEST]   = &ServerHandler::HandleGameJoinRequest;
    _handlerTable[CMSG_MAP_LIST_REQUEST]    = &ServerHandler::HandleMapListRequest;
    _handlerTable[CMSG_GAME_CREATE_REQUEST] = &ServerHandler::HandleGameCreateRequest;
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

            // sGameMgr->Update();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void ServerHandler::HandleUnknown(SessionPtr session, PacketPtr packet)
{
    sLog.out("Invalid opcode (", (uint16_t)packet->getOpcode(), ") received from ", *session);
}

void ServerHandler::HandleHandshakeRequest(SessionPtr session, PacketPtr /*packet*/)
{
    // for now, ignore what is in the magic number of the received packet

    PacketPtr response = PacketPtr(new Packet(SMSG_HANDSHAKE_RESPONSE, 1));
    *response << true;
    session->send(response);
}

void ServerHandler::HandleGameListRequest(SessionPtr /*session*/, PacketPtr /*packet*/)
{
    /*uint32_t length = 4 + sGameMgr->getGamesCount() * (4 + 1);

    for (Game& game : sGameMgr->getGames())
        length += game->getName().length() + 1;

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_LIST_RESPONSE, length));
    *response << sGameMgr->getGamesCount();
    for (GamePtr& game : sGameMgr->getGames())
        *response << game->getId() << game->getName() << game->getPlayerCount();

    session->send(response);
    */
}

void ServerHandler::HandleGameJoinRequest(SessionPtr /*session*/, PacketPtr packet)
{
    uint32_t gameId;
    *packet >> gameId;

    /*
    GamePtr game = sGameMgr->getGameId(gameId);
    ServerPlayerPtr player = nullptr;
    if (game)
        player = game->newPlayer(session);

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_JOIN_RESPONSE, 1 + 1));
    *response << (bool)(player != nullptr);
    *response << (player ? player->getId() : (uint8_t)0);
    */
}

void ServerHandler::HandleMapListRequest(SessionPtr /*session*/, PacketPtr /*packet*/)
{
    /*uint32_t length = 4 + sMapMgr->getMapsCount() * 4;

    for (MapPtr& map : sMapMgr->getMaps())
        length += game->getName().length() + 1;

    PacketPtr response = PacketPtr(new Packet(SMSG_MAP_LIST_RESPONSE, length));

    *response << sMapMgr->getMapsCount();
    for (MapPtr& map : sMapMgr->getMaps())
        *response << map->getId() << map->getName();

    session->send(response);
    */
}

void ServerHandler::HandleGameCreateRequest(SessionPtr /*session*/, PacketPtr packet)
{
    uint32_t mapId;
    std::string gameName;
    *packet >> mapId >> gameName;

    /*
    MapPtr map = sMapMgr->getMapId(mapId);
    GamePtr game = nullptr;
    ServerPlayerPtr player = nullptr;
    if (map)
    {
        game = sGameMgr->newGame(map);
        if (game)
            player = game->addPlayer(session);
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_CREATE_RESPONSE, 1 + 1));
    *response << (bool)(player != nullptr);
    *response << (player ? player->getId() : (uint8_t)0);
    */
}
