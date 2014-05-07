/*
 * level.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
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

    int getWidth() const;
    int getHeight() const;
    bool checkBounds(const Position &pos) const;

    void setId(uint32_t id);
    uint32_t getId() const;
    const std::string& getFilename() const;

    enum class Tile {Forest = 0, Water, Bridge, Grass, Path, Finish, Count};
    Tile getTileAt(const Position &pos) const;
    void setTileAt(const Position &pos, Tile tile);
private:
    char tileToChar(Tile tile);
    Tile charToTile(char c);

    uint32_t _id;
    std::string _filename;
    std::vector<Tile> _data;
    int _width;
    int _height;
};

typedef std::shared_ptr<LevelMap> LevelMapPtr;

#endif /* LEVELMAP_H_ */
