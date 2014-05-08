#include <chrono>
#include "server/ServerHandler.h"
#include "server/GameMgr.h"
#include "server/LevelMapMgr.h"
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
        SessionList toErase = SessionList();

        for (auto itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            SessionPtr& session = *itr;
            if (!session->isConnected())
            {
                toErase.push_back(session);
                continue;
            }

            while (PacketPtr packet = session->getReceivedPacket())
                (this->*_handlerTable[packet->getOpcode()])(session, packet);

            // sGameMgr.Update();
        }

        if (!toErase.empty())
            _server->removeSessions(toErase);

        std::this_thread::sleep_for(std::chrono::milliseconds(SERVER_GAME_SPEED));
    }
}

void ServerHandler::HandleUnknown(SessionPtr session, PacketPtr packet)
{
    sLog.out("Invalid opcode (", (uint16_t)packet->getOpcode(), ") received from ", *session);
}

void ServerHandler::HandleHandshakeRequest(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandleHandshakeRequest ", *session);
    // for now, ignore what is in the magic number of the received packet

    PacketPtr response = PacketPtr(new Packet(SMSG_HANDSHAKE_RESPONSE, 1));
    *response << true;
    session->send(response);
}

void ServerHandler::HandleGameListRequest(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandleGameListRequest ", *session);
    uint32_t length = 4 + sGameMgr.getGamesCount() * (4 + 1 + 2 + 1 + 1);

    for (auto& itr : sGameMgr.getGames())
    {
        ServerGamePtr& game = itr.second;
        LevelMapPtr& map = game->getMap();
        length += game->getName().length() + map->getFilename().length() + 2;
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_LIST_RESPONSE, length));
    *response << sGameMgr.getGamesCount();
    for (auto& itr : sGameMgr.getGames())
    {
        ServerGamePtr& game = itr.second;
        LevelMapPtr& map = game->getMap();
        *response << game->getId() << game->getName() << game->getPlayerCount() << game->getStepTime() << map->getFilename() << map->getWidth() << map->getHeight();
    }

    session->send(response);
}

void ServerHandler::HandleGameJoinRequest(SessionPtr session, PacketPtr packet)
{
    sLog.outDebug("HandleGameJoinRequest ", *session);
    uint32_t gameId;
    *packet >> gameId;

    ServerGamePtr game = sGameMgr.getGameId(gameId);
    LevelMapPtr map = nullptr;
    ServerPlayerPtr player = nullptr;
    if (game)
    {
        player = game->addPlayer(session);
        map = game->getMap();
    }

    bool success = false;
    uint32_t length = 1 + 1;
    std::string mapData = "";

    if (player && map)
    {
        success = true;
        mapData = map->serialize();
        length += mapData.length() + 1;
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_JOIN_RESPONSE, length));
    *response << success;
    if (map && player)
    {
        *response << player->getId();
        *response << mapData;
    }

    session->send(response);
}

void ServerHandler::HandleMapListRequest(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandleMapListRequest ", *session);
    uint32_t length = 4 + sLevelMapMgr.getMapsCount() * (4 + 1 + 1);

    for (auto& itr : sLevelMapMgr.getMaps())
    {
        LevelMapPtr& map = itr.second;
        length += map->getFilename().length() + 1;
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_MAP_LIST_RESPONSE, length));

    *response << sLevelMapMgr.getMapsCount();
    for (auto& itr : sLevelMapMgr.getMaps())
    {
        LevelMapPtr& map = itr.second;
        *response << map->getId() << map->getFilename() << map->getWidth() << map->getHeight();
    }

    session->send(response);
}

void ServerHandler::HandleGameCreateRequest(SessionPtr session, PacketPtr packet)
{
    sLog.outDebug("HandleGameCreateRequest ", *session);
    uint32_t mapId;
    std::string gameName;
    uint16_t stepTime;
    *packet >> mapId >> gameName >> stepTime;

    LevelMapPtr map = sLevelMapMgr.getMapId(mapId);
    ServerGamePtr game = nullptr;
    ServerPlayerPtr player = nullptr;
    if (map)
    {
        game = sGameMgr.newGame(gameName, map, stepTime);
        if (game)
            player = game->addPlayer(session);
    }

    bool success = false;
    uint32_t length = 1 + 1;
    std::string mapData = "";

    if (map && player)
    {
        success = true;
        mapData = map->serialize();
        length += mapData.length() + 1;
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_CREATE_RESPONSE, length));
    *response << success;
    if (map && player)
    {
        *response << player->getId();
        *response << mapData;
    }

    session->send(response);
}
