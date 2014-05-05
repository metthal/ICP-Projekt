#ifndef GAME_H_
#define GAME_H_

#include <list>
#include "common/player.h"
#include "common/sentry.h"
#include "common/levelmap.h"

class Game
{
public:
    Game();
    void update();

    const LevelMap& getMap() const;
    const std::list<Player>& getPlayers() const;
    const Player* getPlayer(int id) const;
    const std::list<Sentry>& getSentries() const;
    int getTime() const;

protected:
	LevelMap _map;
	Position _plankPos;
	bool _plankDropped;
    int _time;
	std::list<Player> _players;
    std::list<Sentry> _sentries;
};

#endif /* GAME_H_ */
