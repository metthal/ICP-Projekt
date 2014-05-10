#include "common/sentry.h"

Sentry::Sentry(uint8_t id)
{
    _id = id;
}

uint8_t Sentry::getId() const
{
    return _id;
}

Direction Sentry::getDirection() const
{
    return _faceDir;
}

Position Sentry::getPosition() const
{
    return _pos;
}

void Sentry::setDirection(Direction dir)
{
    _faceDir = dir;
}

void Sentry::setPosition(Position pos)
{
    _pos = pos;
}
