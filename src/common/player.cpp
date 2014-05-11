/*
 * @file player.cpp
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

#include "common/player.h"

Player::Player(uint8_t id)
{
    _id = id;
    _deaths = 0;
    _hasPlank = false;
    _stepsCount = 0;
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

bool Player::isAlive() const
{
    return _isAlive;
}

uint32_t Player::getStepsCount() const
{
    return _stepsCount;
}

void Player::hasPlank(bool has)
{
    _hasPlank = has;
}

void Player::setDirection(Direction dir)
{
    _faceDir = dir;
}

void Player::setPosition(Position pos)
{
    _pos = pos;
}

void Player::setAlive(bool alive)
{
    _isAlive = alive;
}

void Player::setDeaths(uint32_t deaths)
{
    _deaths = deaths;
}

void Player::setJoinTime(uint32_t joinTime)
{
    _joinTime = joinTime;
}

void Player::setStepsCount(uint32_t count)
{
    _stepsCount = count;
}
