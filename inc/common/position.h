/*
 * position.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
 */

#ifndef POSITION_H_
#define POSITION_H_

enum Direction {Left = 0, Right, Up, Down, Count};

class Position
{
public:
	Position();
	Position(int xx, int yy);
	int linear(int width) const;
	static Position fromLinear(int linearPos, int width);
	Position next(Direction dir);

	bool operator ==(const Position& other) const;
	bool operator !=(const Position& other) const;

	int x;
	int y;
};

#endif /* POSITION_H_ */
