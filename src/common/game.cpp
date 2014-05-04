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
}

const LevelMap& Game::getMap() const
{
    return _map;
}

const std::list<Player>& Game::getPlayers() const
{
    return _players;
}
