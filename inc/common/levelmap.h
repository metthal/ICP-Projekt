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

    enum class Tile {Forest = 0, Water, Bridge, Grass, Path, Count};
	Tile getTileAt(const Position &pos) const;
	void setTileAt(const Position &pos, Tile tile);
private:
	char tileToChar(Tile tile);
	Tile charToTile(char c);

	std::vector<Tile> _data;
	int _width;
	int _height;
};


#endif /* LEVELMAP_H_ */
