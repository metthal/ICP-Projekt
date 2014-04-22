#ifndef CLIENT_H_
#define CLIENT_H_

#include <List>

#include "clientplayer.h"
#include "levelmap.h"

class Client
{
public:
	void loop();
private:
	//TODO Connection _con;
	LevelMap _map;
	std::list<Player> _players;
};

#endif /* CLIENT_H_ */
