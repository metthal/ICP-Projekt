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

}

bool Position::operator ==(const Position& other) const
{
	return x == other.x && y == other.y;
}

bool Position::operator !=(const Position& other) const
{
	return x != other.x || y != other.y;
}
