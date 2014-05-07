/*
 * player.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <cstdint>
#include "common/position.h"

class Player
{
public:
    uint8_t getId() const;
    Direction getDirection() const;
    Position getPosition() const;
    uint32_t getJoinTime() const;
    uint32_t getKills() const;
    bool hasPlank() const;

    void setId(uint8_t id);
    void setDirection(Direction dir);
    void setPosition(Position pos);

protected:
    uint8_t _id;
    Direction _faceDir;
    Position _pos;
    uint32_t _joinTime;
    uint32_t _kills;
    bool _hasPlank;
};

#endif /* PLAYER_H_ */
