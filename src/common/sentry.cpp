/*
 * @file sentry.cpp
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
