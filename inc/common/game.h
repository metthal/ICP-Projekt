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

    /**
     * Returns reference to map.
     * @return Constant reference to map.
     */
    const LevelMap& getMap() const;

    /**
     * Returns list of players in game.
     * @return List of players.
     */
    const std::list<Player>& getPlayers() const;

    /**
     * Returns reference to single player.
     * @param id Id of player to be returned.
     * @return Constant reference to player.
     */
    const Player* getPlayer(int id) const;

    /**
     * Returns list of sentries in game.
     * @return List of sentries.
     */
    const std::list<Sentry>& getSentries() const;

    /**
     * Returns reference to single sentry.
     * @param id Id of sentry to be returned.
     * @return Constant reference to sentry.
     */
    const Sentry* getSentry(int id) const;

    /**
     * Returns time for which the game has been running.
     * @return Game time.
     */
    uint32_t getTime() const;

    /**
     * Tells if plank is dropped.
     * @return True if plank is dropped.
     */
    bool isPlankDropped() const;

    /**
     * Returns the position of plank on map.
     * @return Position of plank.
     */
    Position getPlankPos() const;

    /**
     * Tells if the game is running.
     * @return True if game is running.
     */
    bool isRunning() const;

    /**
     * Loads map from string of characters.
     * @param mapData String representing the map.
     */
    void loadMap(const std::string &mapData);

    /**
     * Ends the game.
     */
    void end();

    /**
     * Sets the current game time in seconds
     * @param gameTime Time in seconds
     */
    void setTime(uint32_t gameTime);

protected:
    LevelMap _map;
    Position _plankPos;
    bool _plankDropped;
    uint32_t _time;
    std::list<Player> _players;
    std::list<Sentry> _sentries;
    bool _running;
};

#endif /* GAME_H_ */
