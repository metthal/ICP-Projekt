#ifndef SENTRY_H
#define SENTRY_H

#include "common/position.h"

class Sentry
{
public:
    Direction getDirection() const;
    Position getPosition() const;

    void setDirection(Direction dir);
    void setPosition(Position pos);
private:
    Direction _faceDir;
    Position _pos;
};

#endif // SENTRY_H
