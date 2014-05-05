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
    int getId() const;
    Direction getDirection() const;
    Position getPosition() const;
    int getJoinTime() const;
    int getKills() const;
    bool hasPlank() const;

    void setId(int id);
    void setDirection(Direction dir);
    void setPosition(Position pos);

protected:
    int _id;
	Direction _faceDir;
	Position _pos;
	int _joinTime;
    int _kills;
	bool _hasPlank;
};

#endif /* PLAYER_H_ */
