/*
 * serverplayer.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
 */

#ifndef SERVERPLAYER_H_
#define SERVERPLAYER_H_

#include "session.h"

class ServerPlayer : public Player
{
public:
	ServerPlayer(LevelMap &levelmap);
	void heartbeat();

private:
	void move(Direction dir);
	void doAction();

	bool _moving;

	Session _session;
	//TODO Connection _con;
}

#endif /* SERVERPLAYER_H_ */
