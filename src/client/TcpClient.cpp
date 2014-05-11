/*
 * @file TcpClient.cpp
 *
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
 * @author Marek Milkovič (xmilko01)
 * @author Ivan Ševčík (xsevci50)
 */

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "client/TcpClient.h"
#include "common/msgexception.h"

TcpClient::TcpClient(const std::string& hostname, uint16_t port) : _thread(), _ioService(), _socket(_ioService), _hostname(hostname), _port(port), _connected(false)
{
    _receivedPackets = new TsQueue<PacketPtr>();
}

TcpClient::~TcpClient()
{
    delete _receivedPackets;
}

void TcpClient::start()
{
    _thread = std::thread(&TcpClient::startImpl, this);

    // wait 2 seconds
    uint32_t timeoutCount = 20;
    while (!_connected)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_WAIT_TIME));
        timeoutCount--;
        if (timeoutCount == 0)
            throw MsgException("TcpClient - unable to connect to the remote endpoint");
    }
}

void TcpClient::startImpl()
{
    try
    {
        startConnect();
        _ioService.run();
    }
    catch (...)
    {
        _connected = false;
        _ioService.stop();
    }
}

void TcpClient::stop()
{
    _ioService.stop();
    _thread.join();
}

void TcpClient::startConnect()
{
    boost::asio::ip::tcp::resolver resolver(_ioService);
    boost::asio::ip::tcp::resolver::query query(_hostname, boost::lexical_cast<std::string>(_port));
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    _socket.async_connect(*endpoint_iterator, boost::bind(&TcpClient::handleConnect, this, boost::asio::placeholders::error));
}

void TcpClient::handleConnect(const boost::system::error_code& error)
{
    if (error)
        throw MsgException("TcpClient::handleConnect - failed to connect to the remote endpoint");

    _connected = true;
    startReceive();
}

void TcpClient::startReceive()
{
    if (!_connected)
        throw MsgException("TcpClient::startReceive - unable to receive from the server due to the connection problems");

    _socket.async_receive(boost::asio::buffer(_buffer, 4096), boost::bind(&TcpClient::handleReceive, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void TcpClient::handleReceive(size_t bytesReceived, const boost::system::error_code& error)
{
    if (error)
        throw MsgException("TcpClient::handleReceive - failed to receive from the server due to connection problems");

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

void TcpClient::send(PacketPtr packet)
{
    if (!_connected)
        throw MsgException("TcpClient::send - unable to communicate with server due to connection problems");

    _socket.async_send(boost::asio::buffer(packet->getBufferCopy()), boost::bind(&TcpClient::handleSend, this, packet, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void TcpClient::handleSend(PacketPtr /*packet*/, size_t /*bytesSent*/, const boost::system::error_code& error)
{
    if (error)
        throw MsgException("TcpClient::handleReceive - failed to receive from the server due to connection problems");
}

PacketPtr TcpClient::getReceivedPacket()
{
    return _receivedPackets->dequeue();
}

PacketPtr TcpClient::getReceivedPacket(uint32_t timeoutMsec)
{
    if (!_connected)
        throw MsgException("TcpClient::getReceivedPacket - client is not connected");

    uint32_t timeoutCount = 0;
    uint32_t maxTimeoutCount = timeoutMsec / PACKET_WAIT_TIME;
    PacketPtr packet = nullptr;
    while (!(packet = _receivedPackets->dequeue()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_WAIT_TIME));

        if (++timeoutCount == maxTimeoutCount)
            throw MsgException("TcpClient::getReceivedPacket - timeout when receiving packet");
    }

    return packet;
}
