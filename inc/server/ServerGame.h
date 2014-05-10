#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <unordered_map>
#include <memory>
#include <random>
#include "server/Session.h"
#include "server/ServerPlayer.h"
#include "common/levelmap.h"
#include "common/game.h"

#define MIN_STEP_TIME           500
#define MAX_STEP_TIME           5000
#define MAX_PLAYER_COUNT        4

typedef std::unordered_map<uint8_t, ServerPlayerPtr> PlayerMap;

class ServerGame
{
public:
    ServerGame() = delete;
    ServerGame(const ServerGame&) = delete;
    ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map);

    ~ServerGame();

    void update(uint32_t diffTime);

    bool setStepTime(uint16_t stepTime);

    uint32_t getId() const;
    const std::string& getName() const;
    LevelMapPtr& getMap();
    uint8_t getPlayerCount() const;
    uint16_t getStepTime() const;
    bool hasFinished() const;
    ServerPlayerPtr getPlayer(uint8_t playerId);

    ServerPlayerPtr addPlayer(SessionPtr& session);
    void spawnPlayer(uint8_t playerId);
    void removePlayer(uint8_t playerId);

private:
    ServerGame& operator =(const ServerGame&);

    void seedFill(std::vector<bool>& output, Position start, std::function<bool (const LevelMap::Tile&)> predicate);
    // First spawn position, around which everything other spawns
    Position getFirstSpawnPos();
    // For spawning sentries
    Position getSentrySpawn();
    // For spawning players and plank (sentries are spawning at random places path / grass / bridge)
    Position getAvailablePos();

    void endGame(uint8_t winnerId);
    void movePlayer(ServerPlayerPtr& player, uint32_t diffTime);
    bool playerCanMoveTo(ServerPlayerPtr& player, const Position& pos);

    uint32_t _id;
    std::string _name;
    LevelMapPtr _map;
    PlayerMap _players;
    uint16_t _stepTime;
    bool _finished;
    std::mt19937 _rng;
    Position _firstSpawnPos;
};

typedef std::shared_ptr<ServerGame> ServerGamePtr;
typedef std::weak_ptr<ServerGame>   ServerGameWptr;

#endif // SERVER_GAME_H
