/*
 * @file GameMgr.h
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

#ifndef GAME_MGR_H
#define GAME_MGR_H

#include <unordered_map>
#include <list>
#include "server/ServerGame.h"
#include "common/levelmap.h"
#include "common/Log.h"

typedef std::unordered_map<uint32_t, ServerGamePtr> GameMap;

class GameMgr
{
public:
    /**
     * Returns singleton class instance.
     * @return Instance of GameMgr.
     */
    static GameMgr& getInstance()
    {
        static GameMgr instance;
        return instance;
    }

    /**
     * Returns number of running games on the server.
     * @return Number of games.
     */
    uint32_t getGamesCount() const
    {
        return _games.size();
    }

    /**
     * Returns list of running games on the server.
     * @return Running games.
     */
    GameMap& getGames()
    {
        return _games;
    }

    /**
     * Returns running game with specified ID.
     * @param gameId ID of the running game.
     * @return Game with specified ID, nullptr if not found.
     */
    ServerGamePtr getGame(uint32_t gameId)
    {
        auto itr = _games.find(gameId);
        if (itr == _games.end())
            return nullptr;

        return itr->second;
    }

    /**
     * Creates new game on the server and inserts it into the list of running games.
     * @param gameName Name of the game.
     * @param map Map used in the game.
     * @param stepTime Period between steps in the game.
     * @param sentryCount Number of sentries in the game.
     * @return Newly created game, nullptr if error occured.
     */
    ServerGamePtr newGame(const std::string& gameName, LevelMapPtr& map, uint16_t stepTime, uint8_t sentryCount)
    {
        auto itr = _games.insert( { _nextGameId, ServerGamePtr(new ServerGame(_nextGameId, gameName, map, sentryCount)) } );
        if (!(itr.first->second->setStepTime(stepTime)))
            return nullptr;

        sLog.out("New game with ID ", itr.first->second->getId(), " running");
        ++_nextGameId;
        return itr.first->second;
    }

    /**
     * Updates all running games on the server.
     * @param diffTime Milliseconds between the last and current update.
     */
    void update(uint32_t diffTime)
    {
        for (auto itr = _games.begin(); itr != _games.end(); )
        {
            ServerGamePtr& game = itr->second;
            if (game->hasFinished() || game->getPlayerCount() == 0)
            {
                game->endGame();
                sLog.out("Game ID ", game->getId(), " removed from the game list");
                itr = _games.erase(itr);
                continue;
            }

            game->update(diffTime);
            ++itr;
        }
    }

private:
    GameMgr() : _nextGameId(1) {}
    GameMgr(const GameMgr&) = delete;
    ~GameMgr() {}

    GameMgr& operator =(const GameMgr&);

    GameMap _games;
    uint32_t _nextGameId;
};

#define sGameMgr GameMgr::getInstance()

#endif // GAME_MGR_H
