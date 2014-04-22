#ifndef GAME_H_
#define GAME_H_

#include <list>
#include "common/player.h"
#include "common/levelmap.h"

class Game
{
public:
    Game();
    void update();

    const LevelMap& getMap() const;
    const std::list<Player>& getPlayers() const;

protected:
	LevelMap _map;
	Position _plankPos;
	bool _plankDropped;
	std::list<Player> _players;
};

#endif /* GAME_H_ */
