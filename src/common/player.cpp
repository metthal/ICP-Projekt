#include "common/player.h"

uint8_t Player::getId() const
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

uint32_t Player::getJoinTime() const
{
    return _joinTime;
}

uint32_t Player::getKills() const
{
    return _kills;
}

bool Player::hasPlank() const
{
    return _hasPlank;
}

void Player::setId(uint8_t id)
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
