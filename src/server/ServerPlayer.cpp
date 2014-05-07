#include "server/ServerPlayer.h"

ServerPlayer::ServerPlayer(uint8_t id, SessionPtr session) : Player(), _session(session)
{
    setId(id);
}

void ServerPlayer::heartbeat()
{
    //TODO get last command sent and perform it
    //TODO command = _con.getLast();

    //TODO resolve command
    //TODO switch(command)
    //TODO case arrowUp: move(Direction::Up);
    //TODO case arrowDown: move(Direction::Down);
    //TODO case arrowLeft: move(Direction::Left);
    //TODO case arrowRight: move(Direction::Right);
    //TODO case actionKey: doAction();
    //TODO case nothing: if (_moving) move(_faceDir);
}

void ServerPlayer::move(Direction dir)
{
    // Change player face direction
    /*_faceDir = dir;

    // Get next position
    Position newPos = _pos.next(dir);

    // Check if can move there
    if (!_session.canMoveTo(newPos))
        return;

    _pos = newPos;*/
    //TODO: notify client
}

void ServerPlayer::doAction()
{
    // Try to build bridge
    /*if (_session.buildBridge(_pos.next(_faceDir)))
        //TODO: notify client
        return;

    // Try to pick up plank
    if (!_hasPlank && _session.pickPlank(_pos.next(_faceDir)))
    {
        _hasPlank = true;
        //TODO: notify client
        return;
    }

    // If no action could be performed, stop movement
    _moving = false;*/
    //TODO: notify client
}
