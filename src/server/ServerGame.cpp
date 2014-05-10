#include "server/ServerGame.h"
#include "common/Log.h"
#include "common/Opcode.h"

ServerGame::ServerGame(uint32_t id, const std::string& name, LevelMapPtr& map) : _id(id), _name(name), _map(map), _players(), _stepTime(0), _finished(false)
{
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

    // cout the length for CREATE, DELETE and UPDATE packets
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
        if (delObjCount)
            player->getSession()->send(deletePacket);

        if (player->getState() == PLAYER_STATE_JUST_JOINED)
            player->getSession()->send(snapshotPacket);
        else
        {
            if (crObjCount)
                player->getSession()->send(createPacket);
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
        itr->second->getSession()->send(updatePacket);
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
    for (newPlayerId = 1; newPlayerId < MAX_PLAYER_COUNT; ++newPlayerId)
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
    // TODO spawn algorithm
    ServerPlayerPtr player = getPlayer(playerId);
    uint8_t count = getPlayerCount();
    for (uint8_t x = 0; x < _map->getWidth(); ++x)
    {
        for (uint8_t y = 0; y < _map->getHeight(); ++y)
        {
            if (_map->getTileAt(Position(x, y)) == LevelMap::Tile::Path)
                if (--count == 0)
                    player->setPosition(Position(x, y));
        }
    }

    player->setState(PLAYER_STATE_ALIVE);
}

void ServerGame::removePlayer(uint8_t playerId)
{
    ServerPlayerPtr player = nullptr;
    if (!(player = getPlayer(playerId)))
        return;

    player->setState(PLAYER_STATE_LEFT_GAME);
    player->getSession()->setState(SESSION_STATE_IN_LOBBY);
}

bool ServerGame::playerCanMoveTo(ServerPlayerPtr& player, const Position& pos)
{
    if (!_map->checkBounds(pos))
        return false;

    if (_map->getTileAt(pos) != LevelMap::Tile::Path)
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
        }
        else
            player->setMoveTime(player->getMoveTime() + diffTime);
    }
}
