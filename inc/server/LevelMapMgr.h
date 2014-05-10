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

#ifndef LEVEL_MAP_MGR
#define LEVEL_MAP_MGR

#include <unordered_map>
#include <boost/filesystem.hpp>
#include "common/levelmap.h"
#include "common/msgexception.h"
#include "common/Log.h"

typedef std::unordered_map<uint32_t, LevelMapPtr> LevelMapList;

class LevelMapMgr
{
public:
    static LevelMapMgr& getInstance()
    {
        static LevelMapMgr instance;
        return instance;
    }

    void loadMaps(const std::string& pathStr)
    {
        sLog.out("Loading maps");
        boost::filesystem::path path(pathStr);
        if (!boost::filesystem::exists(path))
            throw MsgException("LevelMapMgr::loadMaps - cannot find path " + pathStr);

        if (!boost::filesystem::is_directory(path))
            throw MsgException("LevelMapMgr::loadMaps - " + pathStr + " is not a directory");

        boost::filesystem::directory_iterator end;
        for (boost::filesystem::directory_iterator itr(path); itr != end; ++itr)
        {
            if (!boost::filesystem::is_regular_file(itr->status()))
                continue;

            ++_maxMapId;
            auto mapItr = _maps.insert( { _maxMapId, LevelMapPtr(new LevelMap()) } );
            mapItr.first->second->setId(_maxMapId);
            mapItr.first->second->load(itr->path().string());
            sLog.outDebug("Loading map: ", mapItr.first->second->getFilename());
        }

        if (_maps.empty())
            sLog.out("No maps loaded!");
        else
            sLog.out(_maps.size(), " maps loaded");
    }

    uint32_t getMapsCount() const
    {
        return _maps.size();
    }

    LevelMapList& getMaps()
    {
        return _maps;
    }

    LevelMapPtr getMapId(uint32_t mapId)
    {
        auto itr = _maps.find(mapId);
        if (itr == _maps.end())
            return nullptr;

        return itr->second;
    }

private:
    LevelMapMgr() : _maxMapId(0) {}
    LevelMapMgr(const LevelMapMgr&) = delete;
    ~LevelMapMgr() {}

    LevelMapMgr& operator =(const LevelMapMgr&);

    LevelMapList _maps;
    uint32_t _maxMapId;
};

#define sLevelMapMgr LevelMapMgr::getInstance()

#endif // LEVEL_MAP_MGR
