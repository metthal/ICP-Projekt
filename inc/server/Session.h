#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>
#include "common/TsQueue.h"
#include "common/Packet.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session() = delete;
    Session(const Session&) = delete;
    Session(boost::asio::io_service& ioService);

    ~Session();

    void start();
    void send(PacketPtr packet);

    PacketPtr getReceivedPacket();
    boost::asio::ip::tcp::socket& getSocket();

private:
    void startReceive();
    void handleReceive(size_t bytesReceived, const boost::system::error_code& error);
    void handleSend(PacketPtr packet, size_t bytesSent, const boost::system::error_code& error);

    boost::asio::ip::tcp::socket _socket;
    uint8_t _buffer[4096];
    TsQueue<PacketPtr>* _receivedPackets;

    PacketPtr _pendingPacket;
    uint32_t _bytesReserved;
};

typedef std::shared_ptr<Session> SessionPtr;

#endif // SESSION_H
