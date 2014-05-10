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

#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include "client/client.h"
#include "client/TcpClient.h"
#include "common/Opcode.h"
#include "common/Log.h"

int main()
{
    TcpClient client("127.0.0.1", 11337);
    client.start();
    PacketPtr response = nullptr;

    PacketPtr handshake = PacketPtr(new Packet(CMSG_HANDSHAKE_REQUEST, 4));
    *handshake << (uint32_t)1337;
    client.send(handshake);

    while (!(response = client.getReceivedPacket()))
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

    assert(response->getOpcode() == SMSG_HANDSHAKE_RESPONSE);
    sLog.out("Handshake successful!!!");

    std::string command;
    while (std::cin.good())
    {
        std::getline(std::cin, command);
        /*if (command == "handshake")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_HANDSHAKE_REQUEST, 4));
            *packet << (uint32_t)1337;
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_HANDSHAKE_RESPONSE);
            sLog.out("Handshake successful!!!");
        }
        else*/ if (command == "gamelist")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_LIST_REQUEST, 0));
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_GAME_LIST_RESPONSE);
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
        else if (command == "maplist")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_MAP_LIST_REQUEST, 0));
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_MAP_LIST_RESPONSE);
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
        else if (command == "create")
        {
            std::string gameName = "test game";
            PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_CREATE_REQUEST, 4 + gameName.length() + 1 + 2));
            *packet << (uint32_t)1 << gameName << (uint16_t)500;
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_GAME_CREATE_RESPONSE);
            bool success;
            *response >> success;
            if (!success)
            {
                sLog.out("Join to the game failed");
                continue;
            }

            uint8_t playerId;
            std::string mapData;
            *response >> playerId >> mapData;
            sLog.out("Succesfully connected as player ", (uint16_t)playerId);
            sLog.out(mapData);

            /*while (1)
            {
                if (response = client.getReceivedPacket())
                {
                    uint32_t objCount;
                    uint8_t type,id,x,y,rot;
                    if (response->getOpcode() == SMSG_GAME_CREATE_OBJECT)
                    {
                        *response >> objCount;
                        sLog.out("------------CREATE OBJECT-------------");
                        sLog.out("Number of objects: ", objCount);

                        for (uint32_t i = 0; i < objCount; ++i)
                        {
                            *response >> type >> x >> y >> rot >> id;
                            sLog.out("TYPE(", (uint16_t)type, ") [", (uint16_t)x, ",", (uint16_t)y, ",", (uint16_t)rot, "] ID(", (uint16_t)id, ")");
                        }
                        sLog.out("-----------END OF CREATE--------------");
                    }
                    else if (response->getOpcode() == SMSG_GAME_DELETE_OBJECT)
                    {
                        *response >> objCount;
                        sLog.out("------------DELETE OBJECT-------------");
                        sLog.out("Number of objects: ", objCount);

                        for (uint32_t i = 0; i < objCount; ++i)
                        {
                            *response >> type >> id;
                            sLog.out("TYPE(", (uint16_t)type, ") ID(", (uint16_t)id, ")");
                        }
                        sLog.out("-----------END OF DELETE--------------");
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }*/
        }
        else if (command == "join")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_JOIN_REQUEST, 4));
            *packet << (uint32_t)1;
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_GAME_JOIN_RESPONSE);
            bool success;
            *response >> success;
            if (!success)
            {
                sLog.out("Join to the game failed");
                continue;
            }

            uint8_t playerId;
            std::string mapData;
            *response >> playerId >> mapData;
            sLog.out("Succesfully connected as player ", (uint16_t)playerId);
            sLog.out(mapData);

            /*while (1)
            {
                if (response = client.getReceivedPacket())
                {
                    uint32_t objCount;
                    uint8_t type,id,x,y,rot;
                    if (response->getOpcode() == SMSG_GAME_CREATE_OBJECT)
                    {
                        *response >> objCount;
                        sLog.out("------------CREATE OBJECT-------------");
                        sLog.out("Number of objects: ", objCount);

                        for (uint32_t i = 0; i < objCount; ++i)
                        {
                            *response >> type >> x >> y >> rot >> id;
                            sLog.out("TYPE(", (uint16_t)type, ") [", (uint16_t)x, ",", (uint16_t)y, ",", (uint16_t)rot, "] ID(", (uint16_t)id, ")");
                        }
                        sLog.out("-----------END OF CREATE--------------");
                    }
                    else if (response->getOpcode() == SMSG_GAME_DELETE_OBJECT)
                    {
                        *response >> objCount;
                        sLog.out("------------DELETE OBJECT-------------");
                        sLog.out("Number of objects: ", objCount);

                        for (uint32_t i = 0; i < objCount; ++i)
                        {
                            *response >> type >> id;
                            sLog.out("TYPE(", (uint16_t)type, ") ID(", (uint16_t)id, ")");
                        }
                        sLog.out("-----------END OF DELETE--------------");
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }*/
        }
        else if (command == "go")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 1));
            *packet << (uint8_t)PLAYER_ACTION_GO;
            client.send(packet);

            uint8_t x = 255, y = 255;
            uint8_t newX, newY;
            uint64_t currentTime = 0;
            uint64_t oldTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            while (1)
            {
                if (response = client.getReceivedPacket())
                {
                    if (response->getOpcode() == SMSG_GAME_UPDATE_OBJECT)
                    {
                        uint32_t count;
                        *response >> count >> newX >> newY; // placeholders
                        *response >> newX >> newY;
                        if (newX != x || newY != y)
                        {
                            x = newX;
                            y = newY;
                            currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                            sLog.out("Position updated to [",(uint16_t)x,",",(uint16_t)y,"] in ", currentTime - oldTime, " ms");
                            oldTime = currentTime;
                        }
                    }
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        }
        else if (command == "down")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 2));
            *packet << (uint8_t)PLAYER_ACTION_ROTATE_DOWN;
            client.send(packet);
        }
        else if (command == "up")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 2));
            *packet << (uint8_t)PLAYER_ACTION_ROTATE_UP;
            client.send(packet);
        }
        else if (command == "left")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 2));
            *packet << (uint8_t)PLAYER_ACTION_ROTATE_LEFT;
            client.send(packet);
        }
        else if (command == "right")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_PERFORM_ACTION_REQUEST, 2));
            *packet << (uint8_t)PLAYER_ACTION_ROTATE_RIGHT;
            client.send(packet);
        }
        else if (command == "exit")
            break;
    }

    client.stop();
}
