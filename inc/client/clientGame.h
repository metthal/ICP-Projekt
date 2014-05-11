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

    /**
     * Adds player into game.
     * @param id Id of added player.
     */
    void addPlayer(uint8_t id);

    /**
     * Adds sentry into game.
     * @param id Id of added sentry.
     */
    void addSentry(uint8_t id);

    /**
     * Moves player to new position.
     * @param id Id of player.
     * @param pos New position of player.
     * @param dir New direction of player.
     */
    void movePlayer(uint8_t id, Position pos, Direction dir);

    /**
     * Moves sentry to new position.
     * @param id Id of sentry.
     * @param pos New position of sentry.
     * @param dir New direction of sentry.
     */
    void moveSentry(uint8_t id, Position pos, Direction dir);

    /**
     * Sets player game state, namely if he's alive.
     * @param alive Tells if the player is alive or not.
     * @param deaths Number of deaths for statistics.
     */
    void setPlayerState(uint8_t id, bool alive, uint32_t deaths);

    /**
     * Removes player from game.
     * @param id Id of removed player.
     */
    void removePlayer(uint8_t id);

    /**
     * Removes sentry from game.
     * @param id Id of removed sentry.
     */
    void removeSentry(uint8_t id);

    /**
     * Places plank at position and sets it as dropped.
     * @param pos New plank position.
     */
    void placePlank(Position pos);

    /**
     * Removes plank from game as player picks it.
     */
    void removePlank();

    void placeBridge(Position pos);
};

#endif // CLIENTGAME_H
