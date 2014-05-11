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

#include <chrono>
#include "server/ServerHandler.h"
#include "server/GameMgr.h"
#include "server/LevelMapMgr.h"
#include "common/Opcode.h"
#include "common/Log.h"

ServerHandler::ServerHandler(TcpServerPtr& server) : _thread(), _running(false), _server(server)
{
    for (uint8_t opcode = FIRST_OPCODE; opcode < MAX_OPCODE; ++opcode)
        _handlerTable[opcode] = &ServerHandler::HandleUnknown;

    _handlerTable[CMSG_HANDSHAKE_REQUEST]       = &ServerHandler::HandleHandshakeRequest;
    _handlerTable[CMSG_GAME_LIST_REQUEST]       = &ServerHandler::HandleGameListRequest;
    _handlerTable[CMSG_GAME_JOIN_REQUEST]       = &ServerHandler::HandleGameJoinRequest;
    _handlerTable[CMSG_MAP_LIST_REQUEST]        = &ServerHandler::HandleMapListRequest;
    _handlerTable[CMSG_GAME_CREATE_REQUEST]     = &ServerHandler::HandleGameCreateRequest;
    _handlerTable[CMSG_PERFORM_ACTION_REQUEST]  = &ServerHandler::HandlePerformActionRequest;
    _handlerTable[CMSG_PLAYER_LEFT_GAME]        = &ServerHandler::HandlePlayerLeftGame;
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
    uint64_t currentTime = 0;
    uint64_t prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    while (_running)
    {
        SessionList sessions = _server->getSessions();
        SessionList toErase = SessionList();

        for (auto itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            SessionPtr& session = *itr;
            if (session->isConnected())
            {
                while (PacketPtr packet = session->getReceivedPacket())
                {
                    try
                    {
                        if (packet->getOpcode() >= MAX_OPCODE)
                            throw MsgException("Invalid opcode received");

                        (this->*_handlerTable[packet->getOpcode()])(session, packet);
                    }
                    catch (...)
                    {
                        sLog.out("Malformed packet from ", *session, " ignored!");
                        sLog.out(*packet);
                    }
                }
            }
            else
                toErase.push_back(session);
        }

        currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        sGameMgr.update(currentTime - prevTime);
        prevTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

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
    if (session->getState() != SESSION_STATE_AWAITING_HANDSHAKE)
    {
        sLog.outDebug("Session ", *session, " already handshaken and asking for handshake. Ignored.");
        return;
    }

    // for now, ignore what is in the magic number of the received packet

    PacketPtr response = PacketPtr(new Packet(SMSG_HANDSHAKE_RESPONSE, 1));
    *response << true;
    session->send(response);
    session->setState(SESSION_STATE_IN_LOBBY);
}

void ServerHandler::HandleGameListRequest(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandleGameListRequest ", *session);
    if (session->getState() != SESSION_STATE_IN_LOBBY)
    {
        sLog.outDebug("Session ", *session, " not in lobby. Ignored.");
        return;
    }

    uint32_t length = 4 + sGameMgr.getGamesCount() * (4 + 1 + 2 + 1 + 1);

    for (auto& itr : sGameMgr.getGames())
    {
        ServerGamePtr& game = itr.second;
        LevelMap& map = game->getMap();
        length += game->getName().length() + map.getFilename().length() + 2;
    }

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_LIST_RESPONSE, length));
    *response << sGameMgr.getGamesCount();
    for (auto& itr : sGameMgr.getGames())
    {
        ServerGamePtr& game = itr.second;
        LevelMap& map = game->getMap();
        *response << game->getId() << game->getName() << game->getPlayerCount() << game->getStepTime() << map.getFilename() << map.getWidth() << map.getHeight();
    }

    session->send(response);
}

void ServerHandler::HandleGameJoinRequest(SessionPtr session, PacketPtr packet)
{
    sLog.outDebug("HandleGameJoinRequest ", *session);
    if (session->getState() != SESSION_STATE_IN_LOBBY)
    {
        sLog.outDebug("Session ", *session, " not in lobby. Ignored.");
        return;
    }

    uint32_t gameId;
    *packet >> gameId;

    ServerGamePtr game = sGameMgr.getGame(gameId);
    ServerPlayerPtr player = nullptr;
    if (game)
        player = game->addPlayer(session);

    bool success = false;
    uint32_t length = 1 + 1;
    std::string mapData = "";

    if (player)
    {
        LevelMap& map = game->getMap();
        success = true;
        mapData = map.serialize();
        length += mapData.length() + 1;
    }
    else
        session->setState(SESSION_STATE_IN_LOBBY);

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_JOIN_RESPONSE, length));
    *response << success;
    if (player)
    {
        *response << player->getId();
        *response << mapData;
        session->setGameId(game->getId());
        session->setPlayerId(player->getId());
    }
    else
        session->setState(SESSION_STATE_IN_LOBBY);

    session->send(response);
}

void ServerHandler::HandleMapListRequest(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandleMapListRequest ", *session);
    if (session->getState() != SESSION_STATE_IN_LOBBY)
    {
        sLog.outDebug("Session ", *session, " not in lobby. Ignored.");
        return;
    }

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
    if (session->getState() != SESSION_STATE_IN_LOBBY)
    {
        sLog.outDebug("Session ", *session, " not in lobby. Ignored.");
        return;
    }

    uint32_t mapId;
    std::string gameName;
    uint16_t stepTime;
    uint8_t sentryCount;
    *packet >> mapId >> gameName >> stepTime >> sentryCount;

    LevelMapPtr map = sLevelMapMgr.getMapId(mapId);
    ServerGamePtr game = nullptr;
    ServerPlayerPtr player = nullptr;
    if (map)
    {
        game = sGameMgr.newGame(gameName, map, stepTime, sentryCount);
        if (game)
            player = game->addPlayer(session);
    }
    else
        session->setState(SESSION_STATE_IN_LOBBY);

    bool success = false;
    uint32_t length = 1 + 1;
    std::string mapData = "";

    if (map && player)
    {
        success = true;
        mapData = map->serialize();
        length += mapData.length() + 1;
    }
    else
        session->setState(SESSION_STATE_IN_LOBBY);

    PacketPtr response = PacketPtr(new Packet(SMSG_GAME_CREATE_RESPONSE, length));
    *response << success;
    if (map && player)
    {
        *response << player->getId();
        *response << mapData;
        session->setGameId(game->getId());
        session->setPlayerId(player->getId());
    }
    else
        session->setState(SESSION_STATE_IN_LOBBY);

    session->send(response);
}

void ServerHandler::HandlePerformActionRequest(SessionPtr session, PacketPtr packet)
{
    sLog.outDebug("HandlePerformActionRequest ", *session);
    if (session->getState() != SESSION_STATE_IN_GAME)
    {
        sLog.outDebug("Session ", *session, " not in game. Ignored.");
        return;
    }

    ServerGamePtr game = sGameMgr.getGame(session->getGameId());
    if (!game)
        return;

    uint8_t action;
    *packet >> action;

    bool success = game->playerAction(session->getPlayerId(), (PlayerAction)action);

    PacketPtr response = PacketPtr(new Packet(SMSG_PERFORM_ACTION_RESPONSE, 1));
    *response << success;
    session->send(response);
}

void ServerHandler::HandlePlayerLeftGame(SessionPtr session, PacketPtr /*packet*/)
{
    sLog.outDebug("HandlePlayerLeftGame ", *session);
    if (session->getState() != SESSION_STATE_IN_GAME)
    {
        sLog.outDebug("Session ", *session, " not in game. Ignored.");
        return;
    }
    ServerGamePtr game = sGameMgr.getGame(session->getGameId());
    if (!game)
        return;

    game->removePlayer(session->getPlayerId());
}
