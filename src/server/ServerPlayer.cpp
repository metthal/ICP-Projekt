#include "server/ServerPlayer.h"
#include "common/Log.h"

ServerPlayer::ServerPlayer(uint8_t id, SessionPtr& session) : Player(), _session(session)
{
    setId(id);
    _state = PLAYER_STATE_JUST_JOINED;
    _moveTime = _maxMoveTime = 0;
    _respawnTime = 0;
}

void ServerPlayer::update(uint32_t diffTime)
{
    switch (getState())
    {
        case PLAYER_STATE_JUST_JOINED:
            setState(PLAYER_STATE_ALIVE);
            break;
        case PLAYER_STATE_ALIVE:
            if (_moving)
            {
                if (_moveTime + diffTime > _maxMoveTime)
                {
                    move();
                    _moveTime = 0;
                }
                else
                    _moveTime += diffTime;
            }
            else
                _moveTime = 0;
            break;
        case PLAYER_STATE_DEAD:
            if (_respawnTime + diffTime > PLAYER_RESPAWN_TIME)
            {
                setState(PLAYER_STATE_ABOUT_TO_RESPAWN);
                _respawnTime = 0;
            }
            else
                _respawnTime += diffTime;
            break;
        case PLAYER_STATE_ABOUT_TO_RESPAWN:
        case PLAYER_STATE_LEFT_GAME:
        default:
            break;
    }
}

void ServerPlayer::setMaxMoveTime(uint32_t moveTime)
{
    _maxMoveTime = moveTime;
}

void ServerPlayer::setState(PlayerState state)
{
    _state = state;
}

PlayerState ServerPlayer::getState() const
{
    return _state;
}

SessionPtr ServerPlayer::getSession()
{
    return _session.lock();
}

void ServerPlayer::move()
{
    Position newPos = _pos.next(_faceDir);

//    if (!canMoveTo(newPos))
//        return;

    _pos = newPos;
}

bool ServerPlayer::doAction(uint8_t action)
{
    if (getState() != PLAYER_STATE_ALIVE)
        return false;

    switch (action)
    {
        case PLAYER_ACTION_ROTATE_LEFT:
            setDirection(Direction::Left);
            break;
        case PLAYER_ACTION_ROTATE_RIGHT:
            setDirection(Direction::Right);
            break;
        case PLAYER_ACTION_ROTATE_UP:
            setDirection(Direction::Up);
            break;
        case PLAYER_ACTION_ROTATE_DOWN:
            setDirection(Direction::Down);
            break;
        case PLAYER_ACTION_GO:
            _moving = true;
            _moveTime = 0;
            break;
        case PLAYER_ACTION_STOP:
            _moving = false;
            break;
        case PLAYER_ACTION_TAKE:
        case PLAYER_ACTION_OPEN:
            break;
    }

    return true;
}
