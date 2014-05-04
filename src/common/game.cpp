#include "common/game.h"

Game::Game() :
    _plankDropped(true)
{
    _map.load("../examples/level1");

    _players.push_back(Player());
    _players.back().setPosition(Position(2, 2));
    _players.back().setDirection(Direction::Right);
    _players.push_back(Player());
    _players.back().setPosition(Position(5, 2));
    _players.back().setDirection(Direction::Down);
    _players.push_back(Player());
    _players.back().setPosition(Position(2, 5));
    _players.back().setDirection(Direction::Up);
    _players.push_back(Player());
    _players.back().setPosition(Position(5, 5));
    _players.back().setDirection(Direction::Left);

    _sentries.push_back(Sentry());
    _sentries.back().setPosition(Position(3, 2));
    _sentries.back().setDirection(Direction::Right);
    _sentries.push_back(Sentry());
    _sentries.back().setPosition(Position(6, 2));
    _sentries.back().setDirection(Direction::Down);
    _sentries.push_back(Sentry());
    _sentries.back().setPosition(Position(3, 5));
    _sentries.back().setDirection(Direction::Up);
    _sentries.push_back(Sentry());
    _sentries.back().setPosition(Position(6, 5));
    _sentries.back().setDirection(Direction::Left);
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
