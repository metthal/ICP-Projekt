#include "server/ServerGame.h"

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map) : _id(id), _name(name), _map(map)
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

ServerPlayerPtr ServerGame::addPlayer(SessionPtr& session)
{
    ServerPlayerPtr player = ServerPlayerPtr(new ServerPlayer(getPlayerCount(), session));
    _players.push_back(player);
    return player;
}
