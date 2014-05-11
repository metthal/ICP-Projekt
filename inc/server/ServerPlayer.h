/*
 * @file ServerPlayer.h
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
    ServerPlayer(uint8_t id, SessionPtr& _session, uint32_t joinTime);

    /**
     * Updates the players.
     * @param diffTime Milliseconds between last and the current update.
     */
    void update(uint32_t diffTime);

    /**
     * Returns the position on which the player should be if he moved
     * @return Next Position after move.
     */
    Position getPositionAfterMove();

    /**
     * Kills the player.
     */
    void kill();

    /**
     * Fills data about creating of player into the packet.
     * @param packet Packet to fill.
     */
    void buildCreatePacket(PacketPtr& packet);

    /**
     * Fills data about deleting of player into the packet.
     * @param packet Packet to fill.
     */
    void buildDeletePacket(PacketPtr& packet);

    /**
     * Fills data about updating of player into the packet.
     * @param packet Packet to fill.
     */
    void buildUpdatePacket(PacketPtr& packet);

    /**
     * Sets whether player is moving.
     * @param set True if moving, false if not.
     */
    void setMoving(bool set);

    /**
     * Sets the time the player has waited after last step.
     * @param time New time in milliseconds to set.
     */
    void setMoveTime(uint32_t time);

    /**
     * Sets the time the player has waited after the death.
     * @param time New time in milliseconds to set.
     */
    void setRespawnTime(uint32_t time);

    /**
     * Sets the state of the player (if he joined the game, is alive, dead or want to leave).
     * @param PlayerState to set.
     */
    void setState(PlayerState state);

    /**
     * Tells whether player is alive or not.
     * @return True if alive, false if not.
     */
    bool isAlive() const;

    /**
     * Tells whether player is moving or not.
     * @return True if moving, false if not.
     */
    bool isMoving() const;

    /**
     * Tells the time player has waited after last step.
     * @return Time in milliseconds.
     */
    uint32_t getMoveTime() const;

    /**
     * Tells the time player has waited after his death.
     * @return Time in milliseconds.
     */
    uint32_t getRespawnTime() const;

    /**
     * Returns the current state of the player.
     * @return PlayerState of the player.
     */
    PlayerState getState() const;

    /**
     * Returns the sessions asociated with this player.
     * @return Session asociated with player.
     */
    SessionPtr getSession();

private:
    void move();

    bool _moving;
    uint32_t _moveTime;
    uint32_t _respawnTime;

    SessionWptr _session;
    PlayerState _state;
};

typedef std::shared_ptr<ServerPlayer>   ServerPlayerPtr;

#endif /* SERVERPLAYER_H_ */
