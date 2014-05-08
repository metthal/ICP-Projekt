#include "server/ServerGame.h"

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map) : _id(id), _name(name), _map(map), _stepTime(0)
{
}

ServerGame::~ServerGame()
{
}

bool ServerGame::setStepTime(uint16_t stepTime)
{
    if (stepTime < MIN_STEP_TIME || stepTime > MAX_STEP_TIME)
        return false;

    _stepTime = stepTime;
    return true;
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
    if (getPlayerCount() == MAX_PLAYER_COUNT)
        return nullptr;

    uint8_t newPlayerId;
    for (newPlayerId = 1; newPlayerId < MAX_PLAYER_COUNT; ++newPlayerId)
    {
        auto itr = _players.find(newPlayerId);
        if (itr == _players.end())
            break;
    }

    session->setState(SESSION_STATE_IN_GAME);
    auto itr = _players.insert( { newPlayerId, ServerPlayerPtr(new ServerPlayer(newPlayerId, session)) } );
    return itr.first->second;
}
