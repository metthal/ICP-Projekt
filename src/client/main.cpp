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

    std::string command;
    while (std::cin.good())
    {
        std::getline(std::cin, command);
        if (command == "handshake")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_HANDSHAKE_REQUEST, 4));
            *packet << (uint32_t)1337;
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));

            assert(response->getOpcode() == SMSG_HANDSHAKE_RESPONSE);
            sLog.out("Handshake successful!!!");
        }
        else if (command == "gamelist")
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
        }
        else if (command == "join")
        {
            PacketPtr packet = PacketPtr(new Packet(CMSG_GAME_JOIN_REQUEST, 4));
            *packet << (uint32_t)1;
            client.send(packet);

            while (!(response = client.getReceivedPacket()))
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        else if (command == "exit")
            break;
    }

    client.stop();
}
