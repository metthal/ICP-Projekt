#ifndef SESSION_H_
#define SESSION_H_

#include <list>
#include <mutex>

#include "levelmap.h"
#include "position.h"
#include "serverplayer.h"

class Session
{
public:
	Session();

	// Server control, thread-safe
	// Start should be run in new thread
	void start();
	void stop();

	// Game control, thread-safe
	void startGame();


	// Client actions, not thread-safe
	bool canMoveTo(const Position &pos) const;
	bool buildBridge(const Position &pos);
	bool pickPlank(const Position &pos);

private:
	void gameLoop();

	enum class Stage {Lobby, Game};
	Stage _stage;
	bool _private;
	bool _locked;
	// Update interval in milliseconds
	int _interval;


	std::mutex _sessionMutex;
};


#endif /* SESSION_H_ */
