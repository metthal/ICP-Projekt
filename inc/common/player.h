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
    Player(uint8_t id);

    uint8_t getId() const;
    Direction getDirection() const;
    Position getPosition() const;
    uint32_t getJoinTime() const;
    uint32_t getDeaths() const;
    bool hasPlank() const;
    bool isAlive() const;

    void setDirection(Direction dir);
    void setPosition(Position pos);
    void setAlive(bool alive);
    void setDeaths(uint32_t deaths);

protected:
    uint8_t _id;
    Direction _faceDir;
    Position _pos;
    uint32_t _joinTime;
    uint32_t _deaths;
    bool _isAlive;
    bool _hasPlank;
};

#endif /* PLAYER_H_ */
