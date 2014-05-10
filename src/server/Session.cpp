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

#include <boost/bind.hpp>
#include "server/Session.h"
#include "common/Log.h"

Session::Session(boost::asio::io_service& ioService) : _socket(ioService)
{
    _receivedPackets = new TsQueue<PacketPtr>();
    _pendingPacket = nullptr;
    _connected = false;
    _state = SESSION_STATE_AWAITING_HANDSHAKE;
    _bytesReserved = 0;
    memset(_buffer, 0, DEFAULT_BUFFER_SIZE);
    _gameId = 0;
    _playerId = 0;
    _ipAddr = "";
    _port = 0;
}

Session::~Session()
{
    delete _receivedPackets;
}

void Session::start()
{
    _connected = true;
    _ipAddr = _socket.remote_endpoint().address().to_string();
    _port = _socket.remote_endpoint().port();
    sLog.out("Starting new session ", *this);
    startReceive();
}

void Session::startReceive()
{
    _socket.async_receive(boost::asio::buffer(_buffer + _bytesReserved, DEFAULT_BUFFER_SIZE - _bytesReserved), boost::bind(&Session::handleReceive, shared_from_this(),
                boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void Session::setState(SessionState state)
{
    _state = state;
}

void Session::setGameId(uint32_t gameId)
{
    _gameId = gameId;
}

void Session::setPlayerId(uint32_t playerId)
{
    _playerId = playerId;
}

SessionState Session::getState() const
{
    return _state;
}

uint32_t Session::getGameId() const
{
    return _gameId;
}

uint32_t Session::getPlayerId() const
{
    return _playerId;
}

bool Session::isConnected() const
{
    return _connected;
}

void Session::handleReceive(size_t bytesReceived, const boost::system::error_code& error)
{
    if (error)
    {
        _connected = false;
        return;
    }

    uint32_t addedBytes;
    while (bytesReceived >= PACKET_HEADER_SIZE)
    {
        if (!_pendingPacket)
        {
            _pendingPacket = PacketPtr(new Packet(_buffer, bytesReceived));
            addedBytes = _pendingPacket->getCurrentLength();
        }
        else
        {
            addedBytes = _pendingPacket->getCurrentLength();
            _pendingPacket->appendBuffer(_buffer, bytesReceived);
            addedBytes = _pendingPacket->getCurrentLength() - addedBytes;
        }

        memmove(_buffer, _buffer + addedBytes, bytesReceived - addedBytes);
        bytesReceived -= addedBytes;

        if (_pendingPacket->isValid())
        {
            _receivedPackets->enqueue(_pendingPacket);
            _pendingPacket = nullptr;
        }
    }
    _bytesReserved = bytesReceived;

    startReceive();
}

boost::asio::ip::tcp::socket& Session::getSocket()
{
    return _socket;
}

PacketPtr Session::getReceivedPacket()
{
    return _receivedPackets->dequeue();
}

void Session::send(PacketPtr packet)
{
    if (!_connected)
        return;

    _socket.async_send(boost::asio::buffer(packet->getBufferCopy()), boost::bind(&Session::handleSend, this, packet, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void Session::handleSend(PacketPtr /*packet*/, size_t /*bytesSent*/, const boost::system::error_code& error)
{
    if (error)
    {
        _connected = false;
        return;
    }
}

std::ostream& operator <<(std::ostream& stream, const Session& session)
{
    stream << session._ipAddr << ":" << session._port;
    return stream;
}
