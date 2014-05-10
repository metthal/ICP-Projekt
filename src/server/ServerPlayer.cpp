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

#include "server/ServerPlayer.h"
#include "common/Log.h"

ServerPlayer::ServerPlayer(uint8_t id, SessionPtr& session) : Player(id), _session(session)
{
    _state = PLAYER_STATE_JUST_JOINED;
    _moving = false;
    _moveTime = 0;
    _respawnTime = 0;
}

void ServerPlayer::update(uint32_t diffTime)
{
    switch (getState())
    {
        case PLAYER_STATE_JUST_JOINED:
            setState(PLAYER_STATE_ALIVE);
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
        // these handled in ServerGame::update
        case PLAYER_STATE_ALIVE:
        case PLAYER_STATE_ABOUT_TO_RESPAWN:
        case PLAYER_STATE_LEFT_GAME:
        default:
            break;
    }
}

void ServerPlayer::setMoving(bool set)
{
    _moving = set;
}

void ServerPlayer::setMoveTime(uint32_t time)
{
    _moveTime = time;
}

void ServerPlayer::setRespawnTime(uint32_t time)
{
    _respawnTime = time;
}

void ServerPlayer::setState(PlayerState state)
{
    _state = state;
}

bool ServerPlayer::isMoving() const
{
    return _moving;
}

uint32_t ServerPlayer::getMoveTime() const
{
    return _moveTime;
}

uint32_t ServerPlayer::getRespawnTime() const
{
    return _respawnTime;
}

PlayerState ServerPlayer::getState() const
{
    return _state;
}

SessionPtr ServerPlayer::getSession()
{
    return _session.lock();
}

Position ServerPlayer::getPositionAfterMove()
{
    return _pos.next(_faceDir);
}

bool ServerPlayer::doAction(uint8_t action)
{
    if (getState() != PLAYER_STATE_ALIVE)
        return false;

    switch (action)
    {
        case PLAYER_ACTION_ROTATE_LEFT:
            setDirection(Direction::Left);
            _moving = true;
            _moveTime = 0;
            break;
        case PLAYER_ACTION_ROTATE_RIGHT:
            setDirection(Direction::Right);
            _moving = true;
            _moveTime = 0;
            break;
        case PLAYER_ACTION_ROTATE_UP:
            setDirection(Direction::Up);
            _moving = true;
            _moveTime = 0;
            break;
        case PLAYER_ACTION_ROTATE_DOWN:
            setDirection(Direction::Down);
            _moving = true;
            _moveTime = 0;
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
        default:
            return false;
    }

    return true;
}
