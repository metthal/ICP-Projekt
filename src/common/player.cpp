#include "common/player.h"

Player::Player(uint8_t id)
{
    _id = id;
}

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

uint32_t Player::getDeaths() const
{
    return _deaths;
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
