/*
 * @file client.cpp
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

#include "client/client.h"
#include "common/Opcode.h"
#include "common/Log.h"
#include "common/msgexception.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

Client::Client() :
    _tcpClient(nullptr),
    _game(nullptr)
{

}

void Client::start()
{
    const unsigned int interval = 100;

    _close = false;
    std::thread loopThread(&Client::controlLoop, this);
    loopThread.detach();

    while (!_close)
    {
        if (_tcpClient == nullptr)
        { // Waiting for server connection
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            continue;
        }

        PacketPtr response;

        auto t0 = std::chrono::high_resolution_clock::now();

        try { response = _tcpClient->getReceivedPacket(); }
        catch (...) { handleServerDisconnected(); continue; }

        while (response)
        {
            if (_game == nullptr)
            {
                if (response->getOpcode() == SMSG_GAME_LIST_RESPONSE)
                {
                    uint32_t gamesCount;
                    *response >> gamesCount;
                    sLog.out("Number of running games: ", gamesCount);
                    for (uint32_t i = 0; i < gamesCount; ++i)
                    {
                        uint32_t gameId;
                        std::string gameName;
                        uint8_t playerCount;
                        std::string mapName;
                        uint8_t width, height;
                        uint16_t stepTime;
                        *response >> gameId >> gameName >> playerCount >> stepTime >> mapName >> width >> height;

                        sLog.out(gameId, "\t", gameName, "\t", (uint16_t)playerCount, "\t", mapName, "\t", (uint16_t)width, "x", (uint16_t)height, "\t", stepTime);
                    }
                }
                else if (response->getOpcode() == SMSG_MAP_LIST_RESPONSE)
                {
                    uint32_t mapsCount;
                    *response >> mapsCount;
                    sLog.out("Number of available maps: ", mapsCount);
                    for (uint32_t i = 0; i < mapsCount; ++i)
                    {
                        uint32_t mapId;
                        std::string mapName;
                        uint8_t width, height;
                        *response >> mapId >> mapName >> width >> height;

                        sLog.out(mapId, "\t", mapName, "\t", (uint16_t)width, "x", (uint16_t)height);
                    }
                }
                else if (response->getOpcode() == SMSG_GAME_CREATE_RESPONSE)
                {
                    bool success;
                    *response >> success;
                    if (!success)
                    {
                        sLog.out("Join to the game failed");
                        break;
                    }

                    std::string mapData;
                    *response >> _myPlayerId >> mapData;
                    sLog.out("Succesfully connected as player ", (uint16_t)_myPlayerId + 1);
                    sLog.out(mapData);

                    _game = new ClientGame();
                    _game->loadMap(mapData);
                }
                else if (response->getOpcode() == SMSG_GAME_JOIN_RESPONSE)
                {
                    bool success;
                    *response >> success;
                    if (!success)
                    {
                        sLog.out("Join to the game failed");
                        break;
                    }

                    uint8_t playerId;
                    std::string mapData;
                    *response >> playerId >> mapData;
                    sLog.out("Succesfully connected as player ", (uint16_t)_myPlayerId + 1);
                    sLog.out(mapData);

                    _game = new ClientGame();
                    _game->loadMap(mapData);
                }
            }
            else
            { // In game
                if (response->getOpcode() == SMSG_GAME_CREATE_OBJECT)
                {
                    uint32_t count;
                    *response >> count;

                    uint8_t objType;
                    uint8_t posX, posY;
                    uint8_t rotation;
                    uint8_t objId;

                    for (uint32_t i = 0; i < count; i++)
                    {
                        *response >> objType >> posX >> posY;
                        if (objType == OBJECT_TYPE_PLAYER)
                        {
                            bool alive;
                            uint32_t deaths, stepsCount, joinTime;
                            *response >> rotation >> objId >> alive >> deaths >> stepsCount >> joinTime;
                            _game->addPlayer(objId);
                            _game->movePlayer(objId, Position(posX, posY), (Direction)rotation);
                            _game->setPlayerState(objId, alive, deaths, stepsCount);
                            _game->setPlayerJoinTime(objId, joinTime);
                            if (objId == _myPlayerId)
                                sLog.out("You have connected into game.");
                            else
                                sLog.out("Player ", (objId + 1), " connected into game.");
                        }
                        else if (objType == OBJECT_TYPE_SENTRY)
                        {
                            *response >> rotation >> objId;
                            _game->addSentry(objId);
                            _game->moveSentry(objId, Position(posX, posY), (Direction)rotation);
                        }
                        else if (objType == OBJECT_TYPE_PLANK)
                        {
                            _game->placePlank(Position(posX, posY));
                            sLog.out("Plank has been dropped.");
                        }
                        else if (objType == OBJECT_TYPE_BRIDGE)
                        {
                            _game->placeBridge(Position(posX, posY));
                            sLog.out("Bridge has been built.");
                        }
                    }
                }
                else if (response->getOpcode() == SMSG_GAME_UPDATE_OBJECT)
                {
                    uint32_t count;
                    *response >> count;

                    uint8_t objType;
                    uint8_t posX, posY;
                    uint8_t rotation;
                    uint8_t objId;

                    for (uint32_t i = 0; i < count; i++)
                    {
                        *response >> objType >> objId >> posX >> posY >> rotation;
                        if (objType == OBJECT_TYPE_PLAYER)
                        {
                            bool alive, wasAlive;
                            uint32_t deaths, stepsCount;
                            *response >> alive >> deaths >> stepsCount;
                            Position newPos(posX, posY);

                            if (objId == _myPlayerId && (_game->getPlayer(objId)->getPosition() != newPos))
                            {
                                sLog.out("You have moved into [", (uint16_t)posX, " ,", (uint16_t)posY, "]");
                            }

                            _game->movePlayer(objId, Position(posX, posY), (Direction)rotation);
                            wasAlive = _game->getPlayer(objId)->isAlive();
                            if (alive != wasAlive)
                            {
                                if (_myPlayerId == objId)
                                {
                                    if (alive)
                                        sLog.out("You have respawned.");
                                    else
                                        sLog.out("You have been killed.");
                                }
                                else
                                {
                                    if (alive)
                                        sLog.out("Player ", (objId + 1), " has respawned,");
                                    else
                                        sLog.out("Player ", (objId + 1), " has been killed.");
                                }
                            }

                            _game->setPlayerState(objId, alive, deaths, stepsCount);
                        }
                        else if (objType == OBJECT_TYPE_SENTRY)
                        {
                            _game->moveSentry(objId, Position(posX, posY), (Direction)rotation);
                        }
                    }
                }
                else if (response->getOpcode() == SMSG_GAME_DELETE_OBJECT)
                {
                    uint32_t count;
                    *response >> count;

                    uint8_t objType;
                    uint8_t objId;

                    for (uint32_t i = 0; i < count; i++)
                    {
                        *response >> objType;
                        if (objType == OBJECT_TYPE_PLAYER)
                        {
                            *response >> objId;
                            _game->removePlayer(objId);
                            sLog.out("Player ", objId + 1, " has disconnected.");
                        }
                        else if (objType == OBJECT_TYPE_PLANK)
                        {
                            _game->removePlank();
                            sLog.out("Plank has been picked up.");
                        }
                    }
                }
                else if (response->getOpcode() == SMSG_PERFORM_ACTION_RESPONSE)
                {
                    bool success;
                    *response >> success;

                    if (success)
                    {
                        sLog.out("Command successful.");
                    }
                    else
                    {
                        sLog.out("Failed to execute command.");
                        failedCommands++;
                    }
                }
                else if (response->getOpcode() == SMSG_HEARTBEAT)
                {
                    uint32_t gameTime;
                    *response >> gameTime;
                    _game->setTime(gameTime);
                }
            }

            try { response = _tcpClient->getReceivedPacket(); }
            catch (...) { handleServerDisconnected(); break; }
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        auto sleepTime = std::chrono::milliseconds(interval - dur);

        if (sleepTime.count() > 0)
            std::this_thread::sleep_for(sleepTime);
    }

    if (_game != nullptr)
        leaveGame();

    if (_tcpClient != nullptr)
        leaveServer();


}

bool stringToNum(const std::string &str, int &num)
{
    char *endptr;
    // Work with decadic format
    num = strtol(str.c_str(), &endptr, 10);
    if (endptr != str.c_str() + str.length() * sizeof(char))
        return false;
    return true;
}

void Client::controlLoop()
{
    std::string command;

    while (!std::cin.eof())
    {
        std::cin >> command;

        totalCommands++;

        PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 1));

        if (command == "exit")
        {
            _close = true;
            return;
        }

        if (_tcpClient == nullptr)
        { // Selecting server
            if (command == "connect")
            {
                sLog.out("Command connect recognized.");

                int port;
                std::string ip, portStr;
                sLog.out("Enter IP address or hostname.");
                std::cin >> ip;
                sLog.out("Enter port.");
                std::cin >> portStr;
                if (!stringToNum(portStr, port))
                {
                    sLog.out("Invalid port.", true);
                    continue;
                }

                TcpClient *tmpClient = new TcpClient(ip, port);
                try
                {
                    tmpClient->start();

                    // Handshake
                    PacketPtr packet = PacketPtr(new Packet(CMSG_HANDSHAKE_REQUEST, 4));
                    *packet << (uint32_t)1337;
                    tmpClient->send(packet);

                    // Wait second for response
                    PacketPtr response = tmpClient->getReceivedPacket(1000);

                    if (!response || response->getOpcode() != SMSG_HANDSHAKE_RESPONSE)
                        throw MsgException("Server unavailable.");
                }
                catch(...)
                {
                    sLog.out("Unable to connect to server.", true);
                    tmpClient->stop();
                    delete tmpClient; tmpClient = nullptr;
                    continue;
                }

                if (_tcpClient != nullptr)
                    leaveServer();

                _tcpClient = tmpClient;
                sLog.out("Server connected successfully");
            }
            else
            {
                sLog.out("Unknown command.", true);
                continue;
            }
        }
        else if (_game == nullptr)
        { // Selecting / creating game
            if (command == "leave")
            {
                sLog.out("Command leave recognized.");
                leaveServer();
            }
            else if (command == "maplist")
            {
                sLog.out("Command maplist recognized.");
                PacketPtr packet = PacketPtr(new Packet(CMSG_MAP_LIST_REQUEST, 0));
                try { _tcpClient->send(packet); }
                catch (...) { handleServerDisconnected(); continue; }
            }
            else if (command == "gamelist")
            {
                sLog.out("Command gamelist recognized.");
                PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_LIST_REQUEST, 0));
                try { _tcpClient->send(packet); }
                catch (...) { handleServerDisconnected(); continue; }
            }
            else if (command == "create")
            {
                sLog.out("Command create recognized.\nEnter map ID to create game with.");
                std::string idStr;
                int id;
                std::cin >> idStr;
                if (!stringToNum(idStr, id))
                {
                    sLog.out("Invalid id.", true);
                    continue;
                }

                createGame(id);
            }
            else if (command == "join")
            {
                sLog.out("Command join recognized.\nEnter game ID to join.");
                std::string idStr;
                int id;
                std::cin >> idStr;
                if (!stringToNum(idStr, id))
                {
                    sLog.out("Invalid id.", true);
                    continue;
                }

                joinGame(id);
            }
            else
            {
                sLog.out("Unknown command.", true);
                continue;
            }
        }
        else
        { // In game
            totalCommands++;
            if (command == "u" || command == "up")
            {
                *packet << (uint8_t)PLAYER_ACTION_ROTATE_UP;
                sLog.out("Command up recognized.");
            }
            else if (command == "d" || command == "down")
            {
                *packet << (uint8_t)PLAYER_ACTION_ROTATE_DOWN;
                sLog.out("Command down recognized.");
            }
            else if (command == "l" || command == "left")
            {
                *packet << (uint8_t)PLAYER_ACTION_ROTATE_LEFT;
                sLog.out("Command left recognized");
            }
            else if (command == "r" || command == "right")
            {
                *packet << (uint8_t)PLAYER_ACTION_ROTATE_RIGHT;
                sLog.out("Command right recognized.");
            }
            else if (command == "a" || command == "action")
            {
                *packet << (uint8_t)PLAYER_ACTION_ACTION;
                sLog.out("Command action recognized.");
            }
            else if (command == "t" || command == "take")
            {
                *packet << (uint8_t)PLAYER_ACTION_TAKE;
                sLog.out("Command take recognized.");
            }
            else if (command == "o" || command == "open")
            {
                *packet << (uint8_t)PLAYER_ACTION_OPEN;
                sLog.out("Command open recognized.");
            }
            else if (command == "s" || command == "stop")
            {
                *packet << (uint8_t)PLAYER_ACTION_STOP;
                sLog.out("Command stop recognized.");
            }
            else if (command == "g"|| command == "go")
            {
                *packet << (uint8_t)PLAYER_ACTION_GO;
                sLog.out("Command go recognized.");;
            }
            else if (command == "leave")
            {
                sLog.out("Command leave recognized.");
                leaveGame();
                continue;
            }
            else
            {
                failedCommands++;
                sLog.out("Unknown command.", true);
                continue;
            }

            try { _tcpClient->send(packet); }
            catch (...) { handleServerDisconnected(); continue; }
        }
    }
    _close = true;
}

void Client::createGame(int id)
{
    std::string gameName = "test game";
    PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_CREATE_REQUEST, 4 + gameName.length() + 1 + 2 + 1));
    *packet << (uint32_t)id << gameName << (uint16_t)500 << (uint8_t)10;

    try { _tcpClient->send(packet); }
    catch (...) { handleServerDisconnected(); return; }

    failedCommands = totalCommands = 0;
}

void Client::joinGame(int id)
{
    sLog.out("Joining game.");

    PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_JOIN_REQUEST, 4));
    *packet << (uint32_t)id;
    try { _tcpClient->send(packet); }
    catch (...) { handleServerDisconnected(); return; }

    failedCommands = totalCommands = 0;
}

void Client::leaveGame()
{
    sLog.out("Leaving game.");

    delete _game; _game = nullptr;
    PacketPtr packet = PacketPtr(new Packet(CMSG_PLAYER_LEFT_GAME, 0));
    try { _tcpClient->send(packet); }
    catch (...) { handleServerDisconnected(); return; }
}

void Client::leaveServer()
{
    sLog.out("Leaving server.");

    _tcpClient->stop();
    delete _tcpClient; _tcpClient = nullptr;
}

void Client::handleServerDisconnected()
{
    if (_game != nullptr)
        leaveGame();

    _tcpClient->stop();
    delete _tcpClient; _tcpClient = nullptr;

    sLog.out("Server closed unexpectedly.", true);
}
