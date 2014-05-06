#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "client/TcpClient.h"
#include "common/msgexception.h"

TcpClient::TcpClient(const std::string& hostname, uint16_t port) : _thread(), _ioService(), _socket(_ioService), _hostname(hostname), _port(port)
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
}

void TcpClient::startImpl()
{
    startConnect();
    _ioService.run();
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

    startReceive();
}

void TcpClient::startReceive()
{
    _socket.async_receive(boost::asio::buffer(_buffer, 4096), boost::bind(&TcpClient::handleReceive, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void TcpClient::handleReceive(size_t bytesReceived, const boost::system::error_code& error)
{
    if (error)
        return;

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
    _socket.async_send(boost::asio::buffer(packet->getBufferCopy()), boost::bind(&TcpClient::handleSend, this, packet, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void TcpClient::handleSend(PacketPtr /*packet*/, size_t /*bytesSent*/, const boost::system::error_code& error)
{
    if (error)
        return;
}

PacketPtr TcpClient::getReceivedPacket()
{
    return _receivedPackets->dequeue();
}
