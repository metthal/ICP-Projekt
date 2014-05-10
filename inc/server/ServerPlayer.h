#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <memory>
#include "server/Session.h"
#include "common/player.h"

#define PLAYER_RESPAWN_TIME         5000

enum PlayerState
{
    PLAYER_STATE_JUST_JOINED       = 0,
    PLAYER_STATE_ALIVE,
    PLAYER_STATE_DEAD,
    PLAYER_STATE_ABOUT_TO_RESPAWN,
    PLAYER_STATE_LEFT_GAME
};

class ServerPlayer : public Player
{
public:
    ServerPlayer(uint8_t id, SessionPtr& _session);

    void update(uint32_t diffTime);
    bool doAction(uint8_t action);

    void setMaxMoveTime(uint32_t moveTime);
    void setState(PlayerState state);
    PlayerState getState() const;

    SessionPtr getSession();

private:
    void move();

    bool _moving;
    uint32_t _moveTime, _maxMoveTime;
    uint32_t _respawnTime;

    SessionWptr _session;
    PlayerState _state;
};

typedef std::shared_ptr<ServerPlayer>   ServerPlayerPtr;

#endif /* SERVERPLAYER_H_ */
