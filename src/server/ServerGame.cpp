#include "server/ServerGame.h"

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map, uint16_t stepTime) : _id(id), _name(name), _map(map), _stepTime(stepTime)
{
}

ServerGame::~ServerGame()
{
}

uint32_t ServerGame::getId() const
{
    return _id;
}

const std::string& ServerGame::getName() const
{
    return _name;
}

LevelMapPtr& ServerGame::getMap()
{
    return _map;
}

uint8_t ServerGame::getPlayerCount() const
{
    return _players.size();
}

uint16_t ServerGame::getStepTime() const
{
    return _stepTime;
}

ServerPlayerPtr ServerGame::addPlayer(SessionPtr& session)
{
    ServerPlayerPtr player = ServerPlayerPtr(new ServerPlayer(getPlayerCount(), session));
    _players.push_back(player);
    return player;
}
