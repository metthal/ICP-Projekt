#ifndef SERVER_SENTRY_H
#define SERVER_SENTRY_H

#include <memory>
#include "common/sentry.h"

class ServerSentry : public Sentry
{
public:
    ServerSentry(uint8_t id);

    void randomizeMovement();
    void randomizeDirection();
    void randomizeStop();
    Position getPositionAfterMove();

    void setMoving(bool set);
    void setMoveTime(uint32_t time);

    bool isMoving() const;
    uint32_t getMoveTime() const;

private:
    bool _moving;
    uint32_t _moveTime;
};

typedef std::shared_ptr<ServerSentry> ServerSentryPtr;

#endif // SERVER_SENTRY_H
