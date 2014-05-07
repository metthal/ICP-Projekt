#ifndef GAME_MGR_H
#define GAME_MGR_H

#include <unordered_map>
#include <list>
#include "server/ServerGame.h"
#include "common/levelmap.h"

typedef std::unordered_map<uint32_t, ServerGamePtr> GameMap;

class GameMgr
{
public:
    static GameMgr& getInstance()
    {
        static GameMgr instance;
        return instance;
    }

    uint32_t getGamesCount() const
    {
        return _games.size();
    }

    GameMap& getGames()
    {
        return _games;
    }

    ServerGamePtr getGameId(uint32_t gameId)
    {
        auto itr = _games.find(gameId);
        if (itr == _games.end())
            return nullptr;

        return itr->second;
    }

    ServerGamePtr& newGame(const std::string& gameName, LevelMapPtr& map)
    {
        ++_maxGameId;
        auto itr = _games.insert( { _maxGameId, ServerGamePtr(new ServerGame(_maxGameId, gameName, map)) } );
        return itr.first->second;
    }

private:
    GameMgr() : _maxGameId(0) {}
    GameMgr(const GameMgr&) = delete;
    ~GameMgr() {}

    GameMgr& operator =(const GameMgr&);

    GameMap _games;
    uint32_t _maxGameId;
};

#define sGameMgr GameMgr::getInstance()

#endif // GAME_MGR_H
