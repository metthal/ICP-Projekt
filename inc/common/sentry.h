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
    /**
     * Creates new instance of Sentry with specified Id.
     * @param id Identificator that will be used with player.
     */
    Sentry(uint8_t id);

    /**
     * Returns associtated Id of sentry.
     * @return Sentry indentification.
     */
    uint8_t getId() const;

    /**
     * Returns direction in which the sentry is turned.
     * @return Direction for sentry.
     */
    Direction getDirection() const;

    /**
     * Returns position at which the sentry is.
     * @return Position of sentry.
     */
    Position getPosition() const;

    /**
    * Sets the direction in which sentry is turned.
    * @param dir New direction for sentry.
    */
    void setDirection(Direction dir);

    /**
     * Sets position at which the sentry is.
     * @param pos New position for sentry.
     */
    void setPosition(Position pos);

protected:
    uint8_t _id;
    Direction _faceDir;
    Position _pos;
};

#endif // SENTRY_H
