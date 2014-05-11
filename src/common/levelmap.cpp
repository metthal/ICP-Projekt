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

#include "common/levelmap.h"

#include <fstream>
#include <sstream>

#include "common/msgexception.h"

LevelMap::LevelMap()
{
    _id = 0;
    _filename = "";
    _data.clear();
    _finishPos = Position();
    _width = 0;
    _height = 0;
}

LevelMap::LevelMap(const LevelMap& levelmap)
{
    _id = levelmap._id;
    _filename = levelmap._filename;
    _data = levelmap._data;
    _finishPos = levelmap._finishPos;
    _width = levelmap._width;
    _height = levelmap._height;
}

void LevelMap::load(std::string filename)
{
    std::ifstream fin(filename);
    if (!fin.good())
        throw MsgException("Failed to open file: " + filename);

    _filename = filename;
    size_t pos = _filename.rfind("/");
    if (pos != std::string::npos)
        _filename = _filename.substr(pos + 1);

    std::stringstream buffer;
    buffer << fin.rdbuf();

    deserialize(buffer.str());
}

void LevelMap::save(std::string filename)
{
    std::ofstream fout(filename);
    if (!fout.good())
        throw MsgException("Failed to open file: " + filename);

    fout << serialize();
    fout.flush();
}

std::string LevelMap::serialize()
{
    std::stringstream buffer;

    for (uint32_t i = 0; i < _data.size(); i++)
    {
        buffer << tileToChar(_data[i]) << "1";
        if ((uint8_t)(i % _width) == _width - 1)
            buffer << "\n";
        else
            buffer << " ";

    }

    return buffer.str();
}

void LevelMap::deserialize(const std::string& data)
{
    _data.clear();
    ssize_t newLinePos = data.find('\n');
    _width = (newLinePos + 1) / 3;
    _height = (data.length() / 3) / _width;
    for (uint32_t i = 0; i < data.length(); i += 3)
    {
        Tile tile = charToTile(data[i]);

        if (tile == Tile::Finish)
            _finishPos = Position::fromLinear(_data.size(), _width);

        _data.push_back(tile);
    }
}

uint8_t LevelMap::getWidth() const
{
    return _width;
}

uint8_t LevelMap::getHeight() const
{
    return _height;
}

bool LevelMap::checkBounds(const Position &pos) const
{
    return (pos.x >= 0 && pos.x < _width &&
            pos.y >= 0 && pos.y < _height);
}

void LevelMap::setId(uint32_t id)
{
    _id = id;
}

uint32_t LevelMap::getId() const
{
    return _id;
}

const std::string& LevelMap::getFilename() const
{
    return _filename;
}

LevelMap::Tile LevelMap::getTileAt(const Position &pos) const
{
    return _data[pos.linear(_width)];
}

void LevelMap::setTileAt(const Position &pos, Tile tile)
{
    _data[pos.linear(_width)] = tile;
}

bool LevelMap::canWalkOnTile(const Tile& tile) const
{
    return (tile != LevelMap::Tile::Forest && tile != LevelMap::Tile::Water);
}

Position LevelMap::getFinishPos()
{
    return _finishPos;
}

char LevelMap::tileToChar(Tile tile)
{
    switch (tile)
    {
    case Tile::Forest : return 'F';
    case Tile::Water : return 'W';
    case Tile::Bridge : return 'B';
    case Tile::Grass : return 'G';
    case Tile::Path : return 'P';
    case Tile::Finish : return 'X';
    default:
        throw MsgException("Unknown tile");
    }
}

LevelMap::Tile LevelMap::charToTile(char c)
{
    switch (c)
    {
    case 'F': return Tile::Forest;
    case 'W': return Tile::Water;
    case 'B': return Tile::Bridge;
    case 'G': return Tile::Grass;
    case 'P': return Tile::Path;
    case 'X': return Tile::Finish;
    default:
        throw MsgException("Unknown tile");
    }
}
