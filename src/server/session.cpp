#include "session.h"

#include <chrono>

Session::Session() :
	_map(),
	_plankPos(0, 0),
	_plankDropped(true),
	_stage(Stage::Lobby),
	_private(false),
	_locked(false),
	_interval(1000)
{
}

void Session::start()
{
	// Process commands from host
	//TODO remake into passive wait
	while (true)
	{
		switch(_stage)
		{
		case Stage::Lobby:
			_players.front().
			break

		case Stage::Game:
			gameLoop();
			return;
		}
	}
}

void Session::stop()
{
	// Lock session and disconnect all players
	_locked = true;
	for (ServerPlayer x : _players)
	{
		//TODO: disconnect x
	}

	// As no players will be connected afterwards, loop
	// will end and session close
	_players.clear();
}

bool Session::canMoveTo(const Position &pos) const
{
	// Can't move out of map
	if (!_map.checkBounds(pos))
		return false;

	// Can't move on specific tile
	const LevelMap::Tile tile = _map.getTileAt(pos);
	switch (tile)
	{
	case LevelMap::Tile::Forest:
	case LevelMap::Tile::Water:
		return false;
	default:
		return true;
	}
}

bool Session::buildBridge(const Position &pos)
{
	LevelMap::Tile tile = _map.getTileAt(pos);
	if (tile == LevelMap::Tile::Water)
	{
		_map.setTileAt(pos, LevelMap::Tile::Bridge);
		//TODO: notify clients
		return true;
	}
	return false;
}

bool Session::pickPlank(const Position &pos)
{
	// Check if can pick plank there
	if (_plankPos != pos)
		return false;

	_plankDropped = false;

	return true;
}

void Session::gameLoop()
{
	while(_players.size() > 0)
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		_sessionMutex.lock();

		// Let all players update
		for (ServerPlayer x : _players)
		{
			x.heartbeat();
		}

		_sessionMutex.unlock();
		auto t1 = std::chrono::high_resolution_clock::now();

		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

		auto sleepTime = std::chrono::milliseconds(_interval - dur);

		if (sleepTime.count() > 0)
		{
			// Sleep to limit bandwidth
			std::this_thread::sleep_for(sleepTime);
	}
}
