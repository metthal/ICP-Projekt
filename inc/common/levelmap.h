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
#ifndef LEVELMAP_H_
#define LEVELMAP_H_

#include <string>
#include <vector>
#include <memory>

#include "common/position.h"

class LevelMap
{
public:
    void load(std::string filename);
    void save(std::string filename);
    std::string serialize();
    void deserialize(const std::string& data);

    uint8_t getWidth() const;
    uint8_t getHeight() const;
    bool checkBounds(const Position &pos) const;

    void setId(uint32_t id);
    uint32_t getId() const;
    const std::string& getFilename() const;

    enum class Tile {Forest = 0, Water, Bridge, Grass, Path, Finish, Count};
    Tile getTileAt(const Position &pos) const;
    void setTileAt(const Position &pos, Tile tile);
    bool canWalkOnTile(const Tile& tile) const;

    Position getFinishPos();

private:
    char tileToChar(Tile tile);
    Tile charToTile(char c);

    uint32_t _id;
    std::string _filename;
    std::vector<Tile> _data;
    Position _finishPos;
    uint8_t _width;
    uint8_t _height;
};

typedef std::shared_ptr<LevelMap> LevelMapPtr;

#endif /* LEVELMAP_H_ */
