#ifndef SENTRY_H
#define SENTRY_H

#include <cstdint>
#include "common/position.h"

class Sentry
{
public:
    Sentry(uint8_t id);

    uint8_t getId() const;
    Direction getDirection() const;
    Position getPosition() const;

    void setDirection(Direction dir);
    void setPosition(Position pos);

protected:
    uint8_t _id;
    Direction _faceDir;
    Position _pos;
};

#endif // SENTRY_H
