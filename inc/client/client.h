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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <list>
#include <atomic>

#include "common/levelmap.h"
#include "common/player.h"

class Client
{
public:

    /**
     * Starts client.
     */
	void start();

private:
	//TODO Connection _con;
	/**
	 * A control loop function to process commands
	 * from standard input intended to run in separate thread.
	 */
	void controlLoop();
	LevelMap _map;
	std::list<Player> _players;
	std::atomic_bool _close;
};

#endif /* CLIENT_H_ */
