#include "common/player.h"

int Player::getId() const
{
    return _id;
}

Direction Player::getDirection() const
{
    return _faceDir;
}

Position Player::getPosition() const
{
    return _pos;
}

int Player::getJoinTime() const
{
    return _joinTime;
}

int Player::getKills() const
{
    return _kills;
}

bool Player::hasPlank() const
{
	return _hasPlank;
}

void Player::setId(int id)
{
    _id = id;
}

void Player::setDirection(Direction dir)
{
    _faceDir = dir;
}

void Player::setPosition(Position pos)
{
    _pos = pos;
}
