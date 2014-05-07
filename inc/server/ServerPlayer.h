#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <memory>
#include "server/Session.h"
#include "common/player.h"

class ServerPlayer : public Player
{
public:
    ServerPlayer(uint8_t id = 0, SessionPtr _session = nullptr);
    void heartbeat();

private:
    void move(Direction dir);
    void doAction();

    bool _moving;

    SessionPtr _session;
};

typedef std::shared_ptr<ServerPlayer> ServerPlayerPtr;

#endif /* SERVERPLAYER_H_ */
