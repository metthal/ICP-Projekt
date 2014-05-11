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
#include "server/ServerGame.h"
#include "common/Log.h"

ServerPlayer::ServerPlayer(uint8_t id, SessionPtr& session, uint32_t joinTime) : Player(id), _session(session)
{
    _state = PLAYER_STATE_JUST_JOINED;
    _moving = false;
    _moveTime = 0;
    _respawnTime = 0;
    _joinTime = joinTime;
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

void ServerPlayer::buildCreatePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_PLAYER
        << (uint8_t)getPosition().x
        << (uint8_t)getPosition().y
        << (uint8_t)getDirection()
        << (uint8_t)getId()
        << (bool)isAlive()
        << (uint32_t)getDeaths()
        << (uint32_t)getStepsCount()
        << (uint32_t)getJoinTime();
}

void ServerPlayer::buildDeletePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_PLAYER
        << (uint8_t)getId();
}

void ServerPlayer::buildUpdatePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_PLAYER
        << (uint8_t)getId()
        << (uint8_t)getPosition().x
        << (uint8_t)getPosition().y
        << (uint8_t)getDirection()
        << (bool)isAlive()
        << (uint32_t)getDeaths()
        << (uint32_t)getStepsCount();
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

bool ServerPlayer::isAlive() const
{
    return (getState() != PLAYER_STATE_DEAD);
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

void ServerPlayer::kill()
{
    if (!isAlive())
        return;

    setState(PLAYER_STATE_DEAD);
    _respawnTime = 0;
    _moving = false;
    _moveTime = 0;
    _deaths++;
}
