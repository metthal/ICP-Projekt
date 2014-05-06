#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <cstdint>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include "common/Packet.h"
#include "common/TsQueue.h"

class TcpClient
{
public:
    TcpClient() = delete;
    TcpClient(const TcpClient&) = delete;
    TcpClient(const std::string& hostname, uint16_t port);

    ~TcpClient();

    void start();
    void stop();
    void send(PacketPtr packet);

    PacketPtr getReceivedPacket();

private:
    TcpClient& operator=(const TcpClient&);

    void startImpl();
    void startConnect();
    void startReceive();
    void handleConnect(const boost::system::error_code& error);
    void handleReceive(size_t bytesReceived, const boost::system::error_code& error);
    void handleSend(PacketPtr packet, size_t bytesSent, const boost::system::error_code& error);

    std::thread _thread;
    boost::asio::io_service _ioService;
    boost::asio::ip::tcp::socket _socket;
    std::string _hostname;
    uint16_t _port;
    uint8_t _buffer[4096];
    TsQueue<PacketPtr>* _receivedPackets;

    PacketPtr _pendingPacket;
    uint32_t _bytesReserved;
};

#endif // TCP_CLIENT_H
