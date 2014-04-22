#include "clientinterface.h"

void ClientInterface::move(Direction dir)
{
	switch(dir)
	{
	case Direction::Left:
		//TODO: notify server
		break;
	case Direction::Right:
		//TODO: notify server
		break;
	case Direction::Up:
		//TODO: notify server
		break;
	case Direction::Down:
		//TODO: notify server
		break;
	default:
		throw MsgException("Invalid direction");
	}
}

void ClientInterface::action()
{
	//TODO: notify server
}
