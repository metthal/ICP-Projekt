/*
 * @file main.cpp
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

#include <memory>
#include <sstream>
#include "server/TcpServer.h"
#include "server/ServerHandler.h"
#include "server/LevelMapMgr.h"
#include "common/Packet.h"
#include "common/Log.h"

template <typename T> static inline T toNumber(const char* str)
{
    std::stringstream ss(str);
    T val;
    ss >> val;
    return val;
}

int main(int argc, char** argv)
{
    uint16_t port = DEFAULT_SERVER_PORT;
    if (argc >= 2)
    {
        port = toNumber<uint16_t>(argv[1]);
        if (port < MIN_SERVER_PORT)
        {
            sLog.out("Port must be 10000+");
            return 1;
        }
    }

    sLog.out("Welcome to the bludiste2014-server for ICP 2013/2014");
    sLog.out("Starting TCP service on port ", port);

    TcpServerPtr server = std::shared_ptr<TcpServer>(new TcpServer(port));
    ServerHandler handler(server);

    try
    {
        server->start();
        handler.start();

        sLog.out("TCP service running");

        sLevelMapMgr.loadMaps("../examples");
        sLog.out("Server running...");

        server->wait();
        handler.wait();

        sLog.out("Stopping TCP service");
        handler.stop();
        server->stop();
        sLog.out("Terminating server");
    }
    catch (...)
    {
        handler.stop();
        server->stop();
        return 1;
    }

    return 0;
}
