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
    LevelMap();
    LevelMap(const LevelMap& levelmap);

    /**
     * Loads map from file.
     * @param filename File with map data.
     */
    void load(std::string filename);

    /**
     * Saves map to file
     * @param filename File to write map data into.
     */
    void save(std::string filename);

    /**
     * Converts map data into string of characters.
     * @return String representing the map.
     */
    std::string serialize();

    /**
     * Converts string representing the map data into map itself.
     * @param data String representing map data.
     */
    void deserialize(const std::string& data);

    /**
     * Gets width of map.
     * @return Width.
     */
    uint8_t getWidth() const;

    /**
     * Gets height of map.
     * @return Height.
     */
    uint8_t getHeight() const;

    /**
     * Checks if the supplied position is in bounds of map.
     * @param pos Position to check
     * @return True if position is in bounds.
     */
    bool checkBounds(const Position &pos) const;

    /**
     * Sets the Id for map so it can be identified.
     * @param id Identificator for map.
     */
    void setId(uint32_t id);

    /**
     * Gets the Id for map.
     * @return Previously assigned Id.
     */
    uint32_t getId() const;

    /**
     * Returns filename of currently loaded map.
     * @return Filename of current map.
     */
    const std::string& getFilename() const;

    /**
     * Tiles representing the structure of map.
     */
    enum class Tile {Forest = 0, Water, Bridge, Grass, Path, Finish, Count};

    /**
     * Returns tile for specified position.
     * @param pos Position of tile.
     * @return Tile at specified position.
     */
    Tile getTileAt(const Position &pos) const;

    /**
     * Sets tile for specified position.
     * @param pos Position of tile that will be changed.
     * @param tile New tile for position.
     */
    void setTileAt(const Position &pos, Tile tile);

    /**
     * Tells if tile is walkable.
     * Walkable tiles are: Grass, Path, Bridge
     * @return True if tile can be walked.
     */
    bool canWalkOnTile(const Tile& tile) const;

    /**
     * Returns position of finish tile.
     * Reaching it will end the game.
     * @return Position of finish tile.
     */
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
