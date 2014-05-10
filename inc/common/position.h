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
