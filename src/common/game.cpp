/*
 * @file game.cpp
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

#include "common/game.h"

Game::Game() :
    _plankDropped(true),
    _running(true)
{
}

const LevelMap& Game::getMap() const
{
    return _map;
}

const std::list<Player>& Game::getPlayers() const
{
    return _players;
}

const Player* Game::getPlayer(int id) const
{
    for (auto it = _players.begin(); it != _players.end(); it++)
    {
        if (it->getId() == id)
            return &(*it);
    }

    return nullptr;
}

const std::list<Sentry>& Game::getSentries() const
{
    return _sentries;
}

const Sentry* Game::getSentry(int id) const
{
    for (auto it = _sentries.begin(); it != _sentries.end(); it++)
    {
        if (it->getId() == id)
            return &(*it);
    }

    return nullptr;
}

uint32_t Game::getTime() const
{
    return _time;
}


bool Game::isPlankDropped() const
{
    return _plankDropped;
}

Position Game::getPlankPos() const
{
    return _plankPos;
}

bool Game::isRunning() const
{
    return _running;
}

void Game::loadMap(const std::string &mapData)
{
    _map.deserialize(mapData);
}

void Game::end()
{
    _running = false;
}

void Game::setTime(uint32_t gameTime)
{
    _time = gameTime;
}
