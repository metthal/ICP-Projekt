#include "common/levelmap.h"

#include <fstream>
#include <sstream>

#include "common/msgexception.h"


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

    for (unsigned int i = 0; i < _data.size(); i++)
    {
        buffer << tileToChar(_data[i]) << "1";
        if (i % _width == _width - 1)
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
    for (unsigned int i = 0; i < data.length(); i += 3)
        _data.push_back(charToTile(data[i]));
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
