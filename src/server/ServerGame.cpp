#include "server/ServerGame.h"
#include "common/Log.h"
#include "common/Opcode.h"
#include "common/msgexception.h"
#include <stack>
#include <chrono>
#include <cmath>

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map) : _id(id), _name(name), _map(map), _players(), _stepTime(0), _finished(false)
{
    _rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    _firstSpawnPos = getFirstSpawnPos();
}

ServerGame::~ServerGame()
{
}

void ServerGame::update(uint32_t diffTime)
{
    // initialize DELETE_OBJECT and CREATE_OBJECT packets
    //
    // semone picked up plank -> put in DELETE_OBJECT
    // someone build the bridge -> put in CREATE_OBJECT
    //
    // first find all players with state JUST_JOINED/LEFT_GAME -> CREATE_OBJECT/DELETE_OBJECT
    // remove all with LEFT_GAME state
    //
    // initialize UPDATE_OBJECT packets
    // update all players/sentries -> UPDATE_OBJECT
    //
    // send UPDATE_OBJECT packet to every player
    uint32_t delObjCount = 0;
    uint32_t delObjLen = 4;
    uint32_t crObjCount = 0;
    uint32_t crObjLen = 4;
    uint32_t upObjCount = 0;
    uint32_t upObjLen = 4;

    // make the current game create-snapshot
    uint32_t snapObjCount = _players.size();
    uint32_t snapObjLen = 4 + 5 * snapObjCount;
    PacketPtr snapshotPacket = PacketPtr(new Packet(SMSG_GAME_CREATE_OBJECT, snapObjLen));
    *snapshotPacket << snapObjCount;
    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        ServerPlayerPtr& player = itr->second;

        // respawn players that are about to respawn
        if (player->getState() == PLAYER_STATE_ABOUT_TO_RESPAWN)
            spawnPlayer(player->getId());
        else if (!player->getSession()->isConnected() || player->getState() == PLAYER_STATE_LEFT_GAME) // don't include people which are about to leave to snapshot
            continue;

        *snapshotPacket << (uint8_t)OBJECT_TYPE_PLAYER
            << (uint8_t)player->getPosition().x
            << (uint8_t)player->getPosition().y
            << (uint8_t)player->getDirection()
            << (uint8_t)player->getId();
    }

    // count the length for CREATE, DELETE and UPDATE packets
    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        ServerPlayerPtr& player = itr->second;
        if (!player->getSession()->isConnected() || player->getState() == PLAYER_STATE_LEFT_GAME)
        {
            delObjCount++;
            delObjLen += 2;
            continue;
        }
        else if (player->getState() == PLAYER_STATE_JUST_JOINED)
        {
            crObjCount++;
            crObjLen += 5;
        }

        upObjCount++;
        upObjLen += 5;
    }

    PacketPtr deletePacket = PacketPtr(new Packet(SMSG_GAME_DELETE_OBJECT, delObjLen));
    *deletePacket << delObjCount;
    PacketPtr createPacket = PacketPtr(new Packet(SMSG_GAME_CREATE_OBJECT, crObjLen));
    *createPacket << crObjCount;
    PacketPtr updatePacket = PacketPtr(new Packet(SMSG_GAME_UPDATE_OBJECT, upObjLen));
    *updatePacket << upObjCount;

    // build CREATE and DELETE packet
    for (auto itr = _players.begin(); itr != _players.end(); )
    {
        ServerPlayerPtr& player = itr->second;
        if (!player->getSession()->isConnected() || player->getState() == PLAYER_STATE_LEFT_GAME)
        {
            *deletePacket << (uint8_t)OBJECT_TYPE_PLAYER
                << (uint8_t)player->getId();

            itr = _players.erase(itr);
            continue;
        }
        else if (player->getState() == PLAYER_STATE_JUST_JOINED)
        {
            *createPacket << (uint8_t)OBJECT_TYPE_PLAYER
                << (uint8_t)player->getPosition().x
                << (uint8_t)player->getPosition().y
                << (uint8_t)player->getDirection()
                << (uint8_t)player->getId();
        }

        ++itr;
    }

    // send CREATE and DELETE packets
    // perform update and build UPDATE packet
    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        ServerPlayerPtr& player = itr->second;
        try
        {
            if (delObjCount)
                player->getSession()->send(deletePacket);

            if (player->getState() == PLAYER_STATE_JUST_JOINED)
                player->getSession()->send(snapshotPacket);
            else
            {
                if (crObjCount)
                    player->getSession()->send(createPacket);
            }
        }
        catch (...)
        {
            removePlayer(player->getId());
            continue;
        }

        player->update(diffTime);
        movePlayer(player, diffTime);
        *updatePacket << (uint8_t)OBJECT_TYPE_PLAYER
            << (uint8_t)player->getId()
            << (uint8_t)player->getPosition().x
            << (uint8_t)player->getPosition().y
            << (uint8_t)player->getDirection();
    }

    // after update, send UPDATE packets
    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        try
        {
            itr->second->getSession()->send(updatePacket);
        }
        catch (...)
        {
            removePlayer(itr->second->getId());
        }
    }
}

void ServerGame::endGame(uint8_t winnerId)
{
    // send SMSG_GAME_END to all players
    PacketPtr packet = PacketPtr(new Packet(SMSG_GAME_END, 1));
    *packet << winnerId;

    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
        itr->second->getSession()->send(packet);

    sLog.out("Game ID ", getId(), " finished with winner ID ", winnerId);
    _finished = true;
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

bool ServerGame::hasFinished() const
{
    return _finished;
}

ServerPlayerPtr ServerGame::getPlayer(uint8_t playerId)
{
    auto itr = _players.find(playerId);
    if (itr == _players.end())
        return nullptr;

    return itr->second;
}

ServerPlayerPtr ServerGame::addPlayer(SessionPtr& session)
{
    if (getPlayerCount() == MAX_PLAYER_COUNT)
        return nullptr;

    uint8_t newPlayerId;
    for (newPlayerId = 0; newPlayerId < MAX_PLAYER_COUNT; ++newPlayerId)
    {
        if (!getPlayer(newPlayerId))
            break;
    }

    session->setState(SESSION_STATE_IN_GAME);
    auto itr = _players.insert( { newPlayerId, ServerPlayerPtr(new ServerPlayer(newPlayerId, session)) } );
    sLog.out("Player ID ", (uint16_t)itr.first->second->getId(), " (", *(itr.first->second->getSession()), ") joined the game ID ", (uint16_t)getId());
    spawnPlayer(newPlayerId);
    return itr.first->second;
}

void ServerGame::spawnPlayer(uint8_t playerId)
{
    ServerPlayerPtr player = getPlayer(playerId);
    if (!player)
        return;

    player->setState(PLAYER_STATE_ALIVE);
    player->setPosition(getAvailablePos());
    player->setDirection(Direction::Down);
}

void ServerGame::removePlayer(uint8_t playerId)
{
    ServerPlayerPtr player = nullptr;
    if (!(player = getPlayer(playerId)))
        return;

    player->setState(PLAYER_STATE_LEFT_GAME);
    player->getSession()->setState(SESSION_STATE_IN_LOBBY);
}

void ServerGame::seedFill(std::vector<bool>& output, Position start, std::function<bool (const LevelMap::Tile&)> predicate)
{
    output.clear();
    uint8_t height = _map->getHeight(), width = _map->getWidth();
    output.resize((int)height * (int)width);
    std::stack<Position> workingStack;
    workingStack.push(start);

    // Initialize output
    for (auto it = output.begin(); it != output.end(); it++)
        *it = false;

    // Run seed filling
    Position currentPos;
    while (!workingStack.empty())
    {
        currentPos = workingStack.top();
        workingStack.pop();

        if (predicate(_map->getTileAt(currentPos)))
            output[currentPos.linear(width)] = true;
        else
            continue;

        // Add neighbors into stack
        if (currentPos.x > 0)
        {
            Position newPos(currentPos.x - 1, currentPos.y);
            if (!output[newPos.linear(width)])
                workingStack.push(newPos);
        }
        if (currentPos.x < width - 1)
        {
            Position newPos(currentPos.x + 1, currentPos.y);
            if (!output[newPos.linear(width)])
                workingStack.push(newPos);
        }
        if (currentPos.y > 0)
        {
            Position newPos(currentPos.x, currentPos.y - 1);
            if (!output[newPos.linear(width)])
                workingStack.push(newPos);
        }
        if (currentPos.y < height - 1)
        {
            Position newPos(currentPos.x, currentPos.y + 1);
            if (!output[newPos.linear(width)])
                workingStack.push(newPos);
        }
    }
}

Position ServerGame::getFirstSpawnPos()
{
    std::vector<bool> available;
    Position finishPos = _map->getFinishPos();
    seedFill(available, finishPos, [](const LevelMap::Tile& x){return x != LevelMap::Tile::Forest;});

    uint8_t height = _map->getHeight(), width = _map->getWidth();
    int size = (int)height * (int)width;
    for (int i = 0; i < 1000000; i++)
    {
        int linearPos = _rng() % size;
        Position pos = Position::fromLinear(linearPos, width);

        // Test if accessible from finish even over water
        if (available[linearPos])
        {
            // Test if far enough
            if (std::abs(finishPos.x - pos.x) + std::abs(finishPos.y - pos.y) > (height + width) / 4)
            {
                // Test if not already occupied
                bool occupied = false;
                for (auto it = _players.begin(); it != _players.end() && !occupied; it++)
                    occupied = pos == it->second->getPosition();

                //TODO enable these
                /*for (auto it = _sentries.begin(); it != _sentries.end() && !occupied; it++)
                    occupied = pos == it->getPos();
                occupied |= pos == _plankPos;*/

                if (!occupied)
                    return pos;
            }
        }
    }

    throw MsgException("Unable to find first spawn place (bad design of level)");
}

Position ServerGame::getAvailablePos()
{
    std::vector<bool> available;
    Position finishPos = _map->getFinishPos();
    seedFill(available, _firstSpawnPos, [](const LevelMap::Tile& x){return x != LevelMap::Tile::Forest && x != LevelMap::Tile::Water && x != LevelMap::Tile::Finish;});

    uint8_t height = _map->getHeight(), width = _map->getWidth();
    int size = (int)height * (int)width;
    for (int i = 0; i < 1000000; i++)
    {
        int linearPos = _rng() % size;
        Position pos = Position::fromLinear(linearPos, width);

        // Test if accessible from finish even over water
        if (available[linearPos])
        {
            // Test if far enough
            if (std::abs(finishPos.x - pos.x) + std::abs(finishPos.y - pos.y) > (height + width) / 4)
            {
                // Test if not already occupied
                bool occupied = false;
                for (auto it = _players.begin(); it != _players.end() && !occupied; it++)
                    occupied = pos == it->second->getPosition();

                //TODO enable these
                /*for (auto it = _sentries.begin(); it != _sentries.end() && !occupied; it++)
                    occupied = pos == it->getPos();
                occupied |= pos == _plankPos;*/

                if (!occupied)
                    return pos;
            }
        }
    }

    throw MsgException("Unable to find additional spawn place for next object (bad design of level)");
}

bool ServerGame::playerCanMoveTo(ServerPlayerPtr& player, const Position& pos)
{
    if (!_map->checkBounds(pos))
        return false;

    if (!_map->canWalkOnTile(_map->getTileAt(pos)))
        return false;

    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        if (itr->second->getPosition() == pos)
            return false;
    }

    return true;
}

void ServerGame::movePlayer(ServerPlayerPtr& player, uint32_t diffTime)
{
    if (player->getState() != PLAYER_STATE_ALIVE)
        return;

    if (player->isMoving())
    {
        if (player->getMoveTime() + diffTime > _stepTime)
        {
            Position newPos = player->getPositionAfterMove();
            if (!playerCanMoveTo(player, newPos))
            {
                player->setMoving(false);
                player->setMoveTime(0);
                return;
            }

            player->setPosition(newPos);
            player->setMoveTime(0);

            if (_map->getTileAt(newPos) == LevelMap::Tile::Finish)
                endGame(player->getId());
        }
        else
            player->setMoveTime(player->getMoveTime() + diffTime);
    }
}
