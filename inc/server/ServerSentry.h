/*
 * @file ServerSentry.h
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

#ifndef SERVER_SENTRY_H
#define SERVER_SENTRY_H

#include <memory>
#include "common/sentry.h"
#include "common/Packet.h"

class ServerSentry : public Sentry
{
public:
    ServerSentry(uint8_t id);

    /**
     * Generate the random movement (position or direction) of the sentry.
     */
    void randomizeMovement();

    /**
     * Generate the random direction of the sentry.
     */
    void randomizeDirection();

    /**
     * Randomly stops the sentry.
     */
    void randomizeStop();

    /**
     * Returns the position on which the sentry should be if he moved
     * @return Next Position after move.
     */
    Position getPositionAfterMove();

    /**
     * Fills data about creating of sentry into the packet.
     * @param packet Packet to fill.
     */
    void buildCreatePacket(PacketPtr& packet);

    /**
     * Fills data about updating of sentry into the packet.
     * @param packet Packet to fill.
     */
    void buildUpdatePacket(PacketPtr& packet);

    /**
     * Sets whether sentry is moving or not.
     * @param set True if moving, false if not.
     */
    void setMoving(bool set);

    /**
     * Sets the time the sentry has waited after last step.
     * @param time New time in milliseconds to set.
     */
    void setMoveTime(uint32_t time);

    /**
     * Tells whether the sentry is moving or not.
     * @return True if moving, false if not.
     */
    bool isMoving() const;

    /**
     * Tells the time sentry has waited after last step.
     * @return Time in milliseconds.
     */
    uint32_t getMoveTime() const;

private:
    bool _moving;
    uint32_t _moveTime;
};

typedef std::shared_ptr<ServerSentry> ServerSentryPtr;

#endif // SERVER_SENTRY_H
