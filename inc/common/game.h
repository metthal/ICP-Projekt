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

#ifndef GAME_H_
#define GAME_H_

#include <list>
#include "common/player.h"
#include "common/sentry.h"
#include "common/levelmap.h"

enum ObjectType
{
    OBJECT_TYPE_PLAYER      = 0,
    OBJECT_TYPE_SENTRY,
    OBJECT_TYPE_PLANK,
    OBJECT_TYPE_BRIDGE
};

class Game
{
public:
    Game();

    const LevelMap& getMap() const;
    const std::list<Player>& getPlayers() const;
    const Player* getPlayer(int id) const;
    const std::list<Sentry>& getSentries() const;
    const Sentry* getSentry(int id) const;
    int getTime() const;
    bool isPlankDropped() const;
    Position getPlankPos() const;
    bool isRunning() const;

    void loadMap(const std::string &mapData);
    void update();
    void end();

protected:
	LevelMap _map;
	Position _plankPos;
	bool _plankDropped;
    int _time;
	std::list<Player> _players;
    std::list<Sentry> _sentries;
    bool _running;
};

#endif /* GAME_H_ */
