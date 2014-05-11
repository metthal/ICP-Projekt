/*
 * @file Packet.h
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

#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

#define PACKET_HEADER_OPCODE_POS    0
#define PACKET_HEADER_LENGTH_POS    (sizeof(uint8_t))
#define PACKET_HEADER_SIZE          (sizeof(uint8_t) + sizeof(uint32_t))

class Packet
{
public:
    Packet() = delete;

    Packet(uint8_t opcode, uint32_t length);
    Packet(const uint8_t* buffer, uint32_t bufferSize);
    Packet(const Packet& packet);

    ~Packet();

    Packet& operator=(const Packet& packet);

    /**
     * Appends buffer to the packet with the specified size of the buffer. If the size is greater than maximum packet length, only that much data is appended.
     * @param buffer Buffer to append.
     * @param bufferSize Size of the buffer.
     */
    void appendBuffer(const uint8_t* buffer, uint32_t bufferSize);

    /**
     * Return the copy of the packet data.
     * @return The packet buffer copy.
     */
    std::vector<uint8_t> getBufferCopy() const;

    /**
     * Returns the pointer directly to the buffer of the packet.
     * @return Pointer to the packet buffer.
     */
    const uint8_t* getBuffer() const;

    /**
     * Returns the opcode of the packet.
     * @return Opcode of the packet.
     */
    uint8_t getOpcode() const;

    /**
     * Returns the current length of the filled data in the packet.
     * @return Length of the filled data.
     */
    uint32_t getCurrentLength() const;

    /**
     * Returns the length of the whole packet with header.
     * @return Length of the packet.
     */
    uint32_t getLength() const;

    /**
     * Returns the length of the data in the packet.
     * @return Length of the data in the packet.
     */
    uint32_t getDataLength() const;

    /**
     * Tells whether the packet is whole filled or not.
     * @return True if fully filled, false if not.
     */
    bool isValid() const;

    /**
     * Various insertion operators for adding
     * data types into packet.
     */
    Packet& operator<<(const int8_t& data);
    Packet& operator<<(const int16_t& data);
    Packet& operator<<(const int32_t& data);
    Packet& operator<<(const int64_t& data);
    Packet& operator<<(const uint8_t& data);
    Packet& operator<<(const uint16_t& data);
    Packet& operator<<(const uint32_t& data);
    Packet& operator<<(const uint64_t& data);
    Packet& operator<<(const bool& data);
    Packet& operator<<(const char* data);
    Packet& operator<<(const std::string& data);

    /**
     * Various extraction operators for extraction
     * of received data from packet.
     */
    Packet& operator>>(int8_t& data);
    Packet& operator>>(int16_t& data);
    Packet& operator>>(int32_t& data);
    Packet& operator>>(int64_t& data);
    Packet& operator>>(uint8_t& data);
    Packet& operator>>(uint16_t& data);
    Packet& operator>>(uint32_t& data);
    Packet& operator>>(uint64_t& data);
    Packet& operator>>(bool& data);
    Packet& operator>>(std::string& data);

    friend std::ostream& operator <<(std::ostream& stream, const Packet& packet);

private:
    template <typename T> void write(const T& data);
    template <typename T> T read();
    void writeString(const std::string& str);
    void readString(std::string& str);

    uint32_t m_writePos, m_readPos;
    uint32_t m_maxPacketLen;
    std::vector<uint8_t> m_buffer;
};

typedef std::shared_ptr<Packet> PacketPtr;

#endif // PACKET_H
