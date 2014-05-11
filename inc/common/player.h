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
#ifndef PLAYER_H_
#define PLAYER_H_

#include <cstdint>
#include "common/position.h"

/**
 * Enumeration of all possible actions
 * that can player make.
 */
enum PlayerAction
{
    PLAYER_ACTION_ROTATE_LEFT   = 0,
    PLAYER_ACTION_ROTATE_RIGHT,
    PLAYER_ACTION_ROTATE_UP,
    PLAYER_ACTION_ROTATE_DOWN,
    PLAYER_ACTION_GO,
    PLAYER_ACTION_STOP,
    PLAYER_ACTION_TAKE,
    PLAYER_ACTION_OPEN,
    PLAYER_ACTION_ACTION
};

class Player
{
public:
    /**
     * Creates new instance of Player with specified Id.
     * @param id Identificator that will be used with player.
     */
    Player(uint8_t id);

    /**
     * Returns associtated Id of player.
     * @return Player indentification.
     */
    uint8_t getId() const;

    /**
     * Returns direction in which the player is turned.
     * @return Direction for player.
     */
    Direction getDirection() const;

    /**
     * Returns position at which the player is.
     * @return Position of player.
     */
    Position getPosition() const;

    /**
     * Returns time at which player joined game.
     * @return Join time.
     */
    uint32_t getJoinTime() const;

    /**
     * Return how many times the player already died.
     * @return Number of deaths.
     */
    uint32_t getDeaths() const;

    /**
     * Tells if the player is carrying a plank.
     * @return True if player has plank.
     */
    bool hasPlank() const;

    /**
     * Tells if the player is alive.
     * @return True if player is alive.
     */
    bool isAlive() const;

    /**
     * Tells the number of steps player perfmored.
     * @return Number of steps.
     */
    uint32_t getStepsCount() const;

    /**
     * Sets whether player has a plank or not.
     * @param has True if player should have plank.
     */
    void hasPlank(bool has);

    /**
     * Sets the direction in which player is turned.
     * @param dir New direction for player.
     */
    void setDirection(Direction dir);

    /**
     * Sets position at which the player is.
     * @param pos New position for player.
     */
    void setPosition(Position pos);

    /**
     * Sets if the player is alive or not.
     * @param alive Should be true to set player alive.
     */
    void setAlive(bool alive);

    /**
     * Sets how many times the player has died.
     * @param deaths Number of deaths for player.
     */
    void setDeaths(uint32_t deaths);

    /**
     * Sets the time when player joined the game.
     * @param joinTime Time in seconds.
     */
    void setJoinTime(uint32_t joinTime);

    /**
     * Sets the number of steps player did in the game.
     * @param count Number of steps
     */
    void setStepsCount(uint32_t count);

protected:
    uint8_t _id;
    Direction _faceDir;
    Position _pos;
    uint32_t _joinTime;
    uint32_t _deaths;
    bool _isAlive;
    bool _hasPlank;
    uint32_t _stepsCount;
};

#endif /* PLAYER_H_ */
