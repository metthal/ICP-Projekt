/*
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
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

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

void ClientGame::setPlayerState(uint8_t id, bool alive, uint32_t deaths, uint32_t stepsCount)
{
    for (auto it = _players.begin(); it != _players.end(); it++)
    {
        if (it->getId() == id)
        {
            it->setAlive(alive);
            it->setDeaths(deaths);
            it->setStepsCount(stepsCount);
            break;
        }
    }
}

void ClientGame::setPlayerJoinTime(uint8_t id, uint32_t joinTime)
{
    for (auto it = _players.begin(); it != _players.end(); it++)
    {
        if (it->getId() == id)
        {
            it->setJoinTime(joinTime);
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
