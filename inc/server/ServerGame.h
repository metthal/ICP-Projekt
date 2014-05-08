#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <unordered_map>
#include <memory>
#include "server/Session.h"
#include "server/ServerPlayer.h"
#include "common/levelmap.h"

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

    bool setStepTime(uint16_t stepTime);

    uint32_t getId() const;
    const std::string& getName() const;
    LevelMapPtr& getMap();
    uint8_t getPlayerCount() const;
    uint16_t getStepTime() const;

    ServerPlayerPtr addPlayer(SessionPtr& session);

private:
    ServerGame& operator =(const ServerGame&);

    uint32_t _id;
    std::string _name;
    LevelMapPtr _map;
    PlayerMap _players;
    uint16_t _stepTime;
};

typedef std::shared_ptr<ServerGame> ServerGamePtr;

#endif // SERVER_GAME_H
