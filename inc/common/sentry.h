/*
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
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

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
