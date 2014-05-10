#include <chrono>
#include <random>
#include "server/ServerSentry.h"

ServerSentry::ServerSentry(uint8_t id) : Sentry(id), _moving(false), _moveTime(0)
{
}

void ServerSentry::generateMovement()
{
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    uint8_t opt = rng() % 6;
    if (opt <= 3)
    {
        setDirection((Direction)opt);
        _moving = false;
    }
    else
        _moving = true;
}

void ServerSentry::randomizeMovement()
{
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    if (rng() % 6 <= 1)
    {
        _moving = false;
        _moveTime = 0;
    }
    else
    {
        _moving = true;
        _moveTime = 0;
    }
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
