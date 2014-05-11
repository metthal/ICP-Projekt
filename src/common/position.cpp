/*
 * @file position.cpp
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

#include "common/position.h"
#include "common/msgexception.h"

Position::Position() :
    x(0),
    y(0)
{
}

Position::Position(int xx, int yy) :
    x(xx),
    y(yy)
{
}

int Position::linear(int width) const
{
    return x + y * width;
}

Position Position::fromLinear(int linearPos, int width)
{
    return Position(linearPos % width, linearPos / width);
}

Position Position::next(Direction dir)
{
    switch(dir)
    {
    case Direction::Left: return Position(x - 1, y);
    case Direction::Right: return Position(x + 1, y);
    case Direction::Up: return Position(x, y - 1);
    case Direction::Down: return Position(x, y + 1);
    default:
        throw MsgException("Invalid direction");
    }
    return Position(-1, -1);
}

bool Position::operator ==(const Position& other) const
{
    return x == other.x && y == other.y;
}

bool Position::operator !=(const Position& other) const
{
    return x != other.x || y != other.y;
}
