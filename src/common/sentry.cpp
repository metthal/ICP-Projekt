#include "common/sentry.h"

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
