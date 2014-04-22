#include "common/player.h"

Direction Player::getDirection() const
{
    return _faceDir;
}

Position Player::getPosition() const
{
    return _pos;
}

bool Player::hasPlank() const
{
	return _hasPlank;
}

void Player::setDirection(Direction dir)
{
    _faceDir = dir;
}

void Player::setPosition(Position pos)
{
    _pos = pos;
}
