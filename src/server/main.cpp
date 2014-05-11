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
#include "server/TcpServer.h"
#include "server/ServerHandler.h"
#include "server/LevelMapMgr.h"
#include "common/Packet.h"
#include "common/Log.h"

int main()
{
    sLog.out("Welcome to the bludiste2014-server for ICP 2013/2014");
    sLog.out("Starting TCP service on port 11337");

    TcpServerPtr server = std::shared_ptr<TcpServer>(new TcpServer(11337));
    ServerHandler handler(server);

    try
    {
        server->start();
        handler.start();

        sLog.out("TCP service running");

        sLevelMapMgr.loadMaps("../examples");
        sLog.out("Server running...");

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
