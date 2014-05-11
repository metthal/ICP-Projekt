/*
 * @file Session.h
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

#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include "common/TsQueue.h"
#include "common/Packet.h"

#define DEFAULT_BUFFER_SIZE      4096

enum SessionState
{
    SESSION_STATE_AWAITING_HANDSHAKE        = 0,
    SESSION_STATE_IN_LOBBY,
    SESSION_STATE_IN_GAME
};

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session() = delete;
    Session(const Session&) = delete;
    Session(boost::asio::io_service& ioService);

    ~Session();

    /**
     * Starts receiving from the sesssion.
     */
    void start();

    /**
     * Sends the packet to the session.
     * @param packet Packet to send.
     */
    void send(PacketPtr packet);

    /**
     * Sets the state of the session (if it is in lobby, in the game etc)
     * @param SessionState to set.
     */
    void setState(SessionState state);

    /**
     * Sets the ID of the game in which the session has asociated player.
     * @param gameId ID of the game to set.
     */
    void setGameId(uint32_t gameId);

    /**
     * Sets the ID of the player which the session is asociated with.
     */
    void setPlayerId(uint32_t playerId);

    /**
     * Returns the state of the session.
     * @return SessionState of the session.
     */
    SessionState getState() const;

    /**
     * Returns the ID of the game in which the session has asociated player.
     * @return ID of the game.
     */
    uint32_t getGameId() const;

    /**
     * Returns the ID of the player which the session is asociated with.
     * @return ID of the player.
     */
    uint32_t getPlayerId() const;

    /**
     * Tells whether the session is connected or not.
     * @return True if connected, false if not.
     */
    bool isConnected() const;

    /**
     * Returns the first received packet from the queue.
     * @return First receive packet, nullptr if no packet received.
     */
    PacketPtr getReceivedPacket();

    boost::asio::ip::tcp::socket& getSocket();

    friend std::ostream& operator <<(std::ostream& stream, const Session& session);

private:
    void startReceive();
    void handleReceive(size_t bytesReceived, const boost::system::error_code& error);
    void handleSend(PacketPtr packet, size_t bytesSent, const boost::system::error_code& error);

    boost::asio::ip::tcp::socket _socket;
    uint8_t _buffer[DEFAULT_BUFFER_SIZE];
    TsQueue<PacketPtr>* _receivedPackets;

    PacketPtr _pendingPacket;
    uint32_t _bytesReserved;
    std::atomic_bool _connected;
    SessionState _state;
    uint32_t _gameId, _playerId;
    std::string _ipAddr;
    uint16_t _port;
};

typedef std::shared_ptr<Session>    SessionPtr;
typedef std::weak_ptr<Session>      SessionWptr;

#endif // SESSION_H
