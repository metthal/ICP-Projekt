/*
 * player.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "common/position.h"

class Player
{
public:
    Direction getDirection() const;
    Position getPosition() const;
    bool hasPlank() const;

    void setDirection(Direction dir);
    void setPosition(Position pos);

private:
	Direction _faceDir;
	Position _pos;
	bool _hasPlank;
};

#endif /* PLAYER_H_ */
