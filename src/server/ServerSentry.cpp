/*
 * @file ServerSentry.cpp
 *
 * Project name:
 * Bludiste 2014
 *
 * Description:
 * https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
 * https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
 *
 * Project's GitHub repository:
 * https://github.com/metthal/ICP-Projekt
 *
 * Team:
 * @author Marek Milkovič (xmilko01)
 * @author Ivan Ševčík (xsevci50)
 */

#include <chrono>
#include <random>
#include "server/ServerSentry.h"
#include "common/game.h"

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

void ServerSentry::buildCreatePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_SENTRY
        << (uint8_t)getPosition().x
        << (uint8_t)getPosition().y
        << (uint8_t)getDirection()
        << (uint8_t)getId();
}

void ServerSentry::buildUpdatePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_SENTRY
        << (uint8_t)getId()
        << (uint8_t)getPosition().x
        << (uint8_t)getPosition().y
        << (uint8_t)getDirection();
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
