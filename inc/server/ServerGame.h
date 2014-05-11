/*
 * @fileServerGame.h
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

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <unordered_map>
#include <memory>
#include <random>
#include "server/Session.h"
#include "server/ServerPlayer.h"
#include "server/ServerSentry.h"
#include "common/levelmap.h"
#include "common/game.h"

#define MIN_STEP_TIME           500
#define MAX_STEP_TIME           5000
#define MAX_PLAYER_COUNT        4

typedef std::unordered_map<uint8_t, ServerPlayerPtr> PlayerMap;
typedef std::unordered_map<uint8_t, ServerSentryPtr> SentryMap;
typedef std::vector<Position> PositionList;

class ServerGame
{
public:
    ServerGame() = delete;
    ServerGame(const ServerGame&) = delete;
    ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map, uint8_t sentryCount);

    ~ServerGame();

    /**
     * Updates the game and all players in it.
     * @param diffTime Milliseconds between the last and the current update.
     */
    void update(uint32_t diffTime);

    /**
     * Ends the game and remove all players from it.
     */
    void endGame();

    /**
     * Sets the period between steps in the game to the specific time.
     * @param stepTime Time between steps in milliseconds.
     * @return True if step time was set, false if the invalid step time was specified.
     */
    bool setStepTime(uint16_t stepTime);

    /**
     * Returns ID of the game.
     * @return ID of the game.
     */
    uint32_t getId() const;

    /**
     * Returns name of the game.
     * @return Name of the game.
     */
    const std::string& getName() const;

    /**
     * Returns map used in the game.
     * @return Map in the game.
     */
    LevelMap& getMap();

    /**
     * Returns number of players in the game.
     * @return Number of players.
     */
    uint8_t getPlayerCount() const;

    /**
     * Returns period between steps in the game.
     * @return Period between steps in milliseconds.
     */
    uint16_t getStepTime() const;

    /**
     * Tells whether the game has finished.
     * @return True if finished, otherwise false.
     */
    bool hasFinished() const;

    /**
     * Returns player with the specified ID.
     * @return Player with the specified ID, nullptr if not found.
     */
    ServerPlayerPtr getPlayer(uint8_t playerId);

    /**
     * Returns the time for which the game runs.
     * @return Time in seconds.
     */
    uint32_t getGameTime() const;

    /**
     * Adds the player to the game and spawns him.
     * @param session Session which will be asociated with the newly created player.
     * @return Newly created player, nullptr if error occured.
     */
    ServerPlayerPtr addPlayer(SessionPtr& session);

    /**
     * Removes player from the game.
     * @param playerId ID of the player to delete.
     */
    void removePlayer(uint8_t playerId);

    /**
     * Performs action by the specified player.
     * @param playerId ID of the player.
     * @param action PlayerAction to perform.
     * @return True if action was successful, otherwise false.
     */
    bool playerAction(uint8_t playerId, PlayerAction action);

private:
    ServerGame& operator =(const ServerGame&);

    void seedFill(std::vector<bool>& output, Position start, std::function<bool (const LevelMap::Tile&)> predicate);
    // First spawn position, around which everything other spawns
    Position getFirstSpawnPos();
    // For spawning sentries
    Position getSentrySpawn();
    // For spawning players and plank (sentries are spawning at random places path / grass / bridge)
    Position getAvailablePos();

    bool playerPickPlank(ServerPlayerPtr& player);
    bool playerBuildBridge(ServerPlayerPtr& player);

    void spawnPlayer(uint8_t playerId);
    void killPlayer(ServerPlayerPtr& player);
    void movePlayer(ServerPlayerPtr& player, uint32_t diffTime);
    bool playerCanMoveTo(ServerPlayerPtr& player, const Position& pos);

    void moveSentry(ServerSentryPtr& sentry, uint32_t diffTime);
    bool sentryCanMoveTo(ServerSentryPtr& player, const Position& pos);

    void buildPlankPacket(PacketPtr& packet);
    void buildCreateBridgePacket(PacketPtr& paket);

    uint32_t _id;
    std::string _name;
    LevelMap _map;
    PlayerMap _players;
    SentryMap _sentries;
    uint16_t _stepTime;
    bool _finished;
    int8_t _winnerId;
    std::mt19937 _rng;
    Position _firstSpawnPos;

    bool _plankPicked, _plankChanged;
    Position _plankPos;

    bool _newBridge;
    PositionList _bridgePosList;

    uint64_t _gameTime;
};

typedef std::shared_ptr<ServerGame> ServerGamePtr;
typedef std::weak_ptr<ServerGame>   ServerGameWptr;

#endif // SERVER_GAME_H
