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

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <cstdint>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include "common/Packet.h"
#include "common/TsQueue.h"

#define PACKET_WAIT_TIME    100

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
    PacketPtr getReceivedPacket(uint32_t timeoutSecs);

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
    std::atomic_bool _connected;

    PacketPtr _pendingPacket;
    uint32_t _bytesReserved;
};

#endif // TCP_CLIENT_H
