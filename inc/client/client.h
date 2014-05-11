/*
 * @file client.h
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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <list>
#include <string>
#include <atomic>
#include <mutex>

#include "client/TcpClient.h"
#include "client/clientGame.h"


class Client
{
public:
    Client();

    /**
     * Starts client.
     */
    void start();

private:
    /**
     * A control loop function to process commands
     * from standard input intended to run in separate thread.
     */
    void controlLoop();

    void createGame(int id);

    void joinGame(int id);

    void leaveGame();

    void leaveServer();

    void handleServerDisconnected();

    TcpClient *_tcpClient;

    int totalCommands;
    int failedCommands;

    uint8_t _myPlayerId;
    ClientGame *_game;

    std::atomic_bool _close;
};

#endif /* CLIENT_H_ */
