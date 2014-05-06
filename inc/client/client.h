#ifndef CLIENT_H_
#define CLIENT_H_

#include <list>
#include <atomic>

#include "common/levelmap.h"
#include "common/player.h"

class Client
{
public:
	void start();

private:
	//TODO Connection _con;
	void controlLoop();
	LevelMap _map;
	std::list<Player> _players;
	std::atomic_bool _close;
};

#endif /* CLIENT_H_ */
