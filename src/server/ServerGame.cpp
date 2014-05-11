/*
 * @file ServerGame.cpp
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

#include "server/ServerGame.h"
#include "common/Log.h"
#include "common/Opcode.h"
#include "common/msgexception.h"
#include <stack>
#include <chrono>
#include <cmath>

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map, uint8_t sentryCount) : _id(id), _name(name), _map(*map), _players(), _sentries(),
    _stepTime(0), _finished(false), _winnerId(-1)
{
    _rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    _firstSpawnPos = getFirstSpawnPos();
    _plankPos = getAvailablePos();

    for (uint8_t i = 0; i < sentryCount; ++i)
    {
        _sentries[i] = ServerSentryPtr(new ServerSentry(i));
        _sentries[i]->setPosition(getSentrySpawn());
        _sentries[i]->randomizeDirection();
    }

    _plankPicked = false;
    _plankChanged = false;

    _newBridge = false;
    _bridgePosList.clear();

    _gameTime = 0;
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
    uint32_t snapObjCount = _players.size() + _sentries.size() + _bridgePosList.size();
    uint32_t snapObjLen = 4 + (18 * _players.size()) + (5 * _sentries.size()) + (3 * _bridgePosList.size());
    if (!_plankPicked)
    {
        snapObjCount++;
        snapObjLen += 3;
    }

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

        player->buildCreatePacket(snapshotPacket);
    }

    for (auto itr = _sentries.begin(); itr != _sentries.end(); ++itr)
    {
        ServerSentryPtr& sentry = itr->second;
        sentry->buildCreatePacket(snapshotPacket);
    }

    if (!_plankPicked)
        buildPlankPacket(snapshotPacket);

    for (auto itr = _bridgePosList.begin(); itr != _bridgePosList.end(); ++itr)
    {
        buildCreateBridgePacket(snapshotPacket);
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
            crObjLen += 18;
        }

        upObjCount++;
        upObjLen += 14;
    }

    upObjCount += _sentries.size();
    upObjLen += (_sentries.size() * 5);

    if (_plankChanged)
    {
        if (_plankPicked)
        {
            delObjCount++;
            delObjLen += 3;
        }
        else
        {
            crObjCount++;
            crObjLen += 3;
        }
    }

    if (_newBridge)
    {
        crObjCount++;
        crObjLen += 3;
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
            player->buildDeletePacket(deletePacket);
            itr = _players.erase(itr);
            continue;
        }
        else if (player->getState() == PLAYER_STATE_JUST_JOINED)
        {
            player->buildCreatePacket(createPacket);
        }

        ++itr;
    }

    if (_plankChanged)
    {
        if (_plankPicked)
            buildPlankPacket(deletePacket);
        else
            buildPlankPacket(createPacket);

        _plankChanged = false;
    }

    if (_newBridge)
    {
        buildCreateBridgePacket(createPacket);
        _newBridge = false;
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
        player->buildUpdatePacket(updatePacket);
    }

    for (auto itr = _sentries.begin(); itr != _sentries.end(); ++itr)
    {
        ServerSentryPtr& sentry = itr->second;

        moveSentry(sentry, diffTime);
        sentry->buildUpdatePacket(updatePacket);
    }

    // build heartbeat
    _gameTime += diffTime;
    PacketPtr heartbeat = PacketPtr(new Packet(SMSG_HEARTBEAT, 4));
    *heartbeat << (uint32_t)getGameTime();

    // after update, send UPDATE packets
    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        try
        {
            itr->second->getSession()->send(updatePacket);
            itr->second->getSession()->send(heartbeat);
        }
        catch (...)
        {
            removePlayer(itr->second->getId());
        }
    }

}

void ServerGame::endGame()
{
    // send SMSG_GAME_END to all players
    PacketPtr packet = PacketPtr(new Packet(SMSG_GAME_END, 1));
    *packet << _winnerId;

    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        itr->second->getSession()->send(packet);
        removePlayer(itr->second->getId());
    }

    if (_winnerId == -1)
        sLog.out("Game ID ", getId(), " finished with no winner");
    else
        sLog.out("Game ID ", getId(), " finished with winner ID ", (uint16_t)_winnerId);
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

LevelMap& ServerGame::getMap()
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

uint32_t ServerGame::getGameTime() const
{
    return _gameTime / 1000;
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
    auto itr = _players.insert( { newPlayerId, ServerPlayerPtr(new ServerPlayer(newPlayerId, session, getGameTime())) } );
    sLog.out("Player ID ", (uint16_t)itr.first->second->getId(), " (", *(itr.first->second->getSession()), ") joined the game ID ", (uint16_t)getId());
    spawnPlayer(newPlayerId);
    itr.first->second->setState(PLAYER_STATE_JUST_JOINED);
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
    uint8_t height = _map.getHeight(), width = _map.getWidth();
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

        if (predicate(_map.getTileAt(currentPos)))
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
    Position finishPos = _map.getFinishPos();
    seedFill(available, finishPos, [](const LevelMap::Tile& x){return x != LevelMap::Tile::Forest;});

    uint8_t height = _map.getHeight(), width = _map.getWidth();
    int size = (int)height * (int)width;
    for (int i = 0; i < 1000000; i++)
    {
        int linearPos = _rng() % size;
        Position pos = Position::fromLinear(linearPos, width);

        // Test if accessible from finish even over water but not on water!
        if (available[linearPos] && _map.getTileAt(pos) != LevelMap::Tile::Water)
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

Position ServerGame::getSentrySpawn()
{
    std::vector<bool> available;
    Position finishPos = _map.getFinishPos();
    seedFill(available, finishPos, [](const LevelMap::Tile& x){return x != LevelMap::Tile::Forest;});

    uint8_t height = _map.getHeight(), width = _map.getWidth();
    int size = (int)height * (int)width;
    for (int i = 0; i < 1000000; i++)
    {
        int linearPos = _rng() % size;
        Position pos = Position::fromLinear(linearPos, width);

        // Test if accessible from finish even over water but not on water!
        if (available[linearPos] && _map.getTileAt(pos) != LevelMap::Tile::Water)
        {
            // Test if not already occupied
            bool occupied = false;
            for (auto it = _players.begin(); it != _players.end() && !occupied; it++)
                occupied = pos == it->second->getPosition();

            for (auto it = _sentries.begin(); it != _sentries.end() && !occupied; it++)
                occupied = pos == it->second->getPosition();
            occupied |= pos == _plankPos;

            if (!occupied)
                return pos;
        }
    }

    throw MsgException("Unable to find spawn place for sentry (bad design of level)");
}

Position ServerGame::getAvailablePos()
{
    std::vector<bool> available;
    Position finishPos = _map.getFinishPos();
    seedFill(available, _firstSpawnPos, [](const LevelMap::Tile& x){return x != LevelMap::Tile::Forest && x != LevelMap::Tile::Water && x != LevelMap::Tile::Finish;});

    uint8_t height = _map.getHeight(), width = _map.getWidth();
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

                for (auto it = _sentries.begin(); it != _sentries.end() && !occupied; it++)
                    occupied = pos == it->second->getPosition();
                occupied |= pos == _plankPos;

                if (!occupied)
                    return pos;
            }
        }
    }

    throw MsgException("Unable to find additional spawn place for next object (bad design of level)");
}

bool ServerGame::playerCanMoveTo(ServerPlayerPtr& player, const Position& pos)
{
    if (!_map.checkBounds(pos))
        return false;

    if (!_plankPicked && pos == _plankPos)
        return false;

    if (!_map.canWalkOnTile(_map.getTileAt(pos)))
        return false;

    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        if (itr->second->getPosition() == pos)
            return false;
    }

    for (auto itr = _sentries.begin(); itr != _sentries.end(); ++itr)
    {
        if (itr->second->getPosition() == pos)
            killPlayer(player);
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
            player->setStepsCount(player->getStepsCount() + 1);

            if (_map.getTileAt(newPos) == LevelMap::Tile::Finish)
            {
                _finished = true;
                _winnerId = player->getId();
            }
        }
        else
            player->setMoveTime(player->getMoveTime() + diffTime);
    }
}

bool ServerGame::sentryCanMoveTo(ServerSentryPtr& /*sentry*/, const Position& pos)
{
    if (!_map.checkBounds(pos))
        return false;

    if (!_plankPicked && pos == _plankPos)
        return false;

    if (!_map.canWalkOnTile(_map.getTileAt(pos)))
        return false;

    for (auto itr = _sentries.begin(); itr != _sentries.end(); ++itr)
    {
        if (itr->second->getPosition() == pos)
            return false;
    }

    for (auto itr = _players.begin(); itr != _players.end(); ++itr)
    {
        if (itr->second->getPosition() == pos)
        {
            killPlayer(itr->second);
            return !itr->second->hasPlank();
        }
    }

    return true;
}

void ServerGame::moveSentry(ServerSentryPtr& sentry, uint32_t diffTime)
{
    if (sentry->isMoving())
    {
        if (sentry->getMoveTime() + diffTime > _stepTime)
        {
            Position newPos = sentry->getPositionAfterMove();
            if (!sentryCanMoveTo(sentry, newPos))
            {
                sentry->setMoving(false);
                sentry->setMoveTime(0);
                return;
            }

            sentry->setPosition(newPos);
            sentry->setMoveTime(0);
            sentry->randomizeStop();
        }
        else
            sentry->setMoveTime(sentry->getMoveTime() + diffTime);
    }
    else
        sentry->randomizeMovement();
}

bool ServerGame::playerAction(uint8_t playerId, PlayerAction action)
{
    ServerPlayerPtr player = getPlayer(playerId);
    if (!player || player->getState() != PLAYER_STATE_ALIVE)
        return false;

    switch (action)
    {
        case PLAYER_ACTION_ROTATE_LEFT:
            player->setDirection(Direction::Left);
            player->setMoving(true);
            player->setMoveTime(0);
            break;
        case PLAYER_ACTION_ROTATE_RIGHT:
            player->setDirection(Direction::Right);
            player->setMoving(true);
            player->setMoveTime(0);
            break;
        case PLAYER_ACTION_ROTATE_UP:
            player->setDirection(Direction::Up);
            player->setMoving(true);
            player->setMoveTime(0);
            break;
        case PLAYER_ACTION_ROTATE_DOWN:
            player->setDirection(Direction::Down);
            player->setMoving(true);
            player->setMoveTime(0);
            break;
        case PLAYER_ACTION_GO:
            player->setMoving(true);
            player->setMoveTime(0);
            break;
        case PLAYER_ACTION_STOP:
            player->setMoving(false);
            break;
        case PLAYER_ACTION_TAKE:
            return playerPickPlank(player);
        case PLAYER_ACTION_OPEN:
            return playerBuildBridge(player);
        case PLAYER_ACTION_ACTION:
        {
            bool success = playerPickPlank(player);
            if (!success)
                success = playerBuildBridge(player);

            return success;
        }
        default:
            return false;
    }

    return true;
}

void ServerGame::killPlayer(ServerPlayerPtr& player)
{
    if (!player->isAlive())
        return;

    if (player->hasPlank())
    {
        player->hasPlank(false);
        _plankPos = player->getPosition();
        _plankPicked = false;
        _plankChanged = true;
    }

    player->kill();
}

bool ServerGame::playerPickPlank(ServerPlayerPtr& player)
{
    if (!player->isAlive())
        return false;

    Position nextPos = player->getPositionAfterMove();
    if (!_plankPicked && nextPos == _plankPos)
    {
        player->hasPlank(true);
        _plankPicked = true;
        _plankChanged = true;
        return true;
    }

    return false;
}

bool ServerGame::playerBuildBridge(ServerPlayerPtr& player)
{
    if (!player->isAlive())
        return false;

    if (!player->hasPlank())
        return false;

    Position nextPos = player->getPositionAfterMove();
    if (_map.getTileAt(nextPos) == LevelMap::Tile::Water)
    {
        player->hasPlank(false);
        _plankPicked = false;
        _plankChanged = true;
        _map.setTileAt(nextPos, LevelMap::Tile::Bridge);
        _plankPos = getAvailablePos();
        _newBridge = true;
        _bridgePosList.push_back(nextPos);
        return true;
    }

    return false;
}

void ServerGame::buildPlankPacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_PLANK
        << (uint8_t)_plankPos.x
        << (uint8_t)_plankPos.y;
}

void ServerGame::buildCreateBridgePacket(PacketPtr& packet)
{
    *packet << (uint8_t)OBJECT_TYPE_BRIDGE
        << (uint8_t)_bridgePosList.back().x
        << (uint8_t)_bridgePosList.back().y;
}
