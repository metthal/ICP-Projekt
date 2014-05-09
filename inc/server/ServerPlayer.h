#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <memory>
#include "server/Session.h"
#include "common/player.h"

class ServerPlayer : public Player
{
public:
    ServerPlayer(uint8_t id, SessionPtr& _session);
    void heartbeat();

private:
    void move(Direction dir);
    void doAction();

    bool _moving;

    SessionWptr _session;
};

typedef std::shared_ptr<ServerPlayer> ServerPlayerPtr;

#endif /* SERVERPLAYER_H_ */
