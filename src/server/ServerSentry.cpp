#include <chrono>
#include <random>
#include "server/ServerSentry.h"

ServerSentry::ServerSentry(uint8_t id) : Sentry(id), _moving(false), _moveTime(0)
{
}

void ServerSentry::randomizeMovement()
{
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    if (rng() % 2)
        randomizeDirection();
    else
        randomizeStop();
}

void ServerSentry::randomizeDirection()
{
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    setDirection((Direction)(rng() % 4));
}

void ServerSentry::randomizeStop()
{
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    if (rng() % 4)
        _moving = true;
    else
        _moving = false;
}

Position ServerSentry::getPositionAfterMove()
{
    return _pos.next(_faceDir);
}

void ServerSentry::setMoving(bool set)
{
    _moving = set;
}

void ServerSentry::setMoveTime(uint32_t time)
{
    _moveTime = time;
}

bool ServerSentry::isMoving() const
{
    return _moving;
}

uint32_t ServerSentry::getMoveTime() const
{
    return _moveTime;
}
