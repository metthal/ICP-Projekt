#include <memory>
#include "server/TcpServer.h"
#include "server/ServerHandler.h"
#include "server/LevelMapMgr.h"
#include "common/Packet.h"
#include "common/Log.h"

int main()
{
    sLevelMapMgr.loadMaps("../examples");

    sLog.out("Welcome to the bludiste2014-server for ICP 2013/2014");
    sLog.out("Starting TCP service on port 11337");

    TcpServerPtr server = std::shared_ptr<TcpServer>(new TcpServer(11337));
    ServerHandler handler(server);

    server->start();
    handler.start();

    sLog.out("TCP service running");

    std::string command;
    while (std::cin.good())
    {
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        else
            sLog.out("Unknown command '", command, "'");
    }

    sLog.out("Stopping TCP service");
    handler.stop();
    server->stop();
    sLog.out("Terminating server");
    return 0;
}
