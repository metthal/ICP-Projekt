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

#ifndef CLIENTGAME_H
#define CLIENTGAME_H

#include "common/game.h"

class ClientGame : public Game
{
public:
    ClientGame();

    void addPlayer(uint8_t id);
    void addSentry(uint8_t id);

    void movePlayer(uint8_t id, Position pos, Direction dir);
    void moveSentry(uint8_t id, Position pos, Direction dir);

    void removePlayer(uint8_t id);
    void removeSentry(uint8_t id);

    void placePlank(Position pos);
    void removePlank();

    void placeBridge(Position pos);
};

#endif // CLIENTGAME_H
