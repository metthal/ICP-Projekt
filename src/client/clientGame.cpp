#include "client/clientGame.h"

ClientGame::ClientGame()
{
}

void ClientGame::addPlayer(uint8_t id)
{
    _players.push_back(Player(id));
}

void ClientGame::addSentry(uint8_t id)
{
    _sentries.push_back(Sentry(id));
}

void ClientGame::movePlayer(uint8_t id, Position pos, Direction dir)
{
    for (auto it = _players.begin(); it != _players.end(); it++)
    {
        if (it->getId() == id)
        {
            it->setPosition(pos);
            it->setDirection(dir);
            break;
        }
    }
}

void ClientGame::moveSentry(uint8_t id, Position pos, Direction dir)
{
    for (auto it = _sentries.begin(); it != _sentries.end(); it++)
    {
        if (it->getId() == id)
        {
            it->setPosition(pos);
            it->setDirection(dir);
            break;
        }
    }
}

void ClientGame::removePlayer(uint8_t id)
{
    _players.remove_if([id](const Player& x){return x.getId() == id;});
}

void ClientGame::removeSentry(uint8_t id)
{
    _sentries.remove_if([id](const Sentry& x){return x.getId() == id;});
}

void ClientGame::placePlank(Position pos)
{
    _plankDropped = true;
    _plankPos = pos;
}

void ClientGame::removePlank()
{
    _plankDropped = false;
}

void ClientGame::placeBridge(Position pos)
{
    _map.setTileAt(pos, LevelMap::Tile::Bridge);
}
