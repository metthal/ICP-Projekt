#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <vector>
#include <memory>
#include "server/Session.h"
#include "server/ServerPlayer.h"
#include "common/levelmap.h"

typedef std::vector<ServerPlayerPtr> PlayerList;

class ServerGame
{
public:
    ServerGame() = delete;
    ServerGame(const ServerGame&) = delete;
    ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map, uint16_t stepTime);

    ~ServerGame();

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
    PlayerList _players;
    uint16_t _stepTime;
};

typedef std::shared_ptr<ServerGame> ServerGamePtr;

#endif // SERVER_GAME_H
