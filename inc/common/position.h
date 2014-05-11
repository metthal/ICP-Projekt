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
#ifndef POSITION_H_
#define POSITION_H_

enum Direction {Left = 0, Right, Up, Down, Count};

class Position
{
public:
	Position();

	/**
	 * Creates new instance of Position with specified coordinates.
	 * @param xx X coordinate for position.
	 * @param yy Y coordinate for position.
	 */
	Position(int xx, int yy);

	/**
	 * Converts position from two dimensional space into one dimensional
	 * linear space.
	 * @param width Width of two dimensional space.
	 * @return Position in linear space.
	 */
	int linear(int width) const;

	/**
	 * Converts position from one dimensional linear space into two
	 * dimensional one
	 * @param linearPos Linear position to be converted.
	 * @param width Width of two dimensional space.
	 * @return Position in two dimensional space.
	 */
	static Position fromLinear(int linearPos, int width);

	/**
	 * Returns next position in specified direction.
	 * @param dir Direction in which to move.
	 * @return Position that follows previous one in specified direction.
	 */
	Position next(Direction dir);

	/**
	 * Operator for equality of two positions.
	 * @param other The second position to compare with.
	 * @return True if this and other position are equal.
	 */
	bool operator ==(const Position& other) const;

	/**
     * Operator for inequality of two positions.
     * @param other The second position to compare with.
     * @return True if this and other position are not equal.
     */
	bool operator !=(const Position& other) const;

	int x;
	int y;
};

#endif /* POSITION_H_ */
