/*
 * @file Packet.cpp
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

#include <cstring>
#include <iomanip>
#include "common/Packet.h"
#include "common/msgexception.h"

Packet::Packet(uint8_t opcode, uint32_t length) : m_writePos(0), m_readPos(PACKET_HEADER_SIZE)
{
    m_maxPacketLen = PACKET_HEADER_SIZE + length;
    m_buffer.resize(m_maxPacketLen, 0);
    write<uint8_t>(opcode);
    write<uint32_t>(length);
}

Packet::Packet(const uint8_t* buffer, uint32_t bufferSize)
{
    m_readPos = PACKET_HEADER_SIZE;
    m_writePos = 0;

    uint32_t pktLen = *((uint32_t*)&buffer[PACKET_HEADER_LENGTH_POS]) + PACKET_HEADER_SIZE;
    m_maxPacketLen = pktLen;
    m_buffer.resize(pktLen);
    appendBuffer(buffer, bufferSize);
}

Packet::Packet(const Packet& packet) : m_writePos(packet.m_writePos), m_readPos(packet.m_readPos),  m_maxPacketLen(packet.m_maxPacketLen), m_buffer(packet.m_buffer.begin(), packet.m_buffer.end())
{
}

Packet::~Packet() { }

Packet& Packet::operator=(const Packet& packet)
{
    m_writePos = packet.m_writePos;
    m_readPos = packet.m_readPos;
    m_maxPacketLen = packet.m_maxPacketLen;
    m_buffer = std::vector<uint8_t>(packet.m_buffer.begin(), packet.m_buffer.end());
    return *this;
}

void Packet::appendBuffer(const uint8_t* buffer, uint32_t bufferSize)
{
    uint32_t bytesToCopy = std::min(m_maxPacketLen - m_writePos, bufferSize);
    memcpy(&m_buffer[m_writePos], buffer, bytesToCopy);
    m_writePos += bytesToCopy;
}

std::vector<uint8_t> Packet::getBufferCopy() const
{
    return m_buffer;
}

const uint8_t* Packet::getBuffer() const
{
    return &m_buffer[0];
}

uint8_t Packet::getOpcode() const
{
    return m_buffer[PACKET_HEADER_OPCODE_POS];
}

uint32_t Packet::getCurrentLength() const
{
    return m_writePos;
}

uint32_t Packet::getLength() const
{
    return getDataLength() + PACKET_HEADER_SIZE;
}

uint32_t Packet::getDataLength() const
{
    return *((uint32_t*)&m_buffer[PACKET_HEADER_LENGTH_POS]);
}

uint32_t Packet::getBufferSize() const
{
    return m_buffer.size();
}

bool Packet::isValid() const
{
    return getLength() == m_writePos;
}

template <typename T> void Packet::write(const T& data)
{
    size_t dataSize = sizeof(T);
    if (m_writePos + dataSize > m_maxPacketLen)
        throw MsgException("Packet::write - data exceeds packet length");

    memcpy(&m_buffer[m_writePos], &data, dataSize);
    m_writePos += dataSize;
}

void Packet::writeString(const std::string& str)
{
    if (m_writePos + str.length() + 1 > m_maxPacketLen)
        throw MsgException("Packet::writeString - data exceeds packet length");

    memcpy(&m_buffer[m_writePos], str.c_str(), str.length() + 1);
    m_writePos += str.length() + 1;
}

template <typename T> T Packet::read()
{
    if (m_readPos + sizeof(T) > m_maxPacketLen)
        throw MsgException("Packet::read - reading exceeds packet length");

    T& data = *((T*)&m_buffer[m_readPos]);
    m_readPos += sizeof(T);
    return data;
}

void Packet::readString(std::string& str)
{
    str = std::string();
    while (m_buffer[m_readPos] != '\0')
    {
        str += m_buffer[m_readPos++];
        if (m_readPos > m_maxPacketLen)
            throw MsgException("Packet::readString - reading exceeds packet length");
    }

    m_readPos++;
}

std::ostream& operator <<(std::ostream& stream, const Packet& packet)
{
    std::ios_base::fmtflags flags = stream.flags();
    stream << "OPCODE: " << (uint16_t)packet.getOpcode() << " LENGTH: " << packet.getDataLength() << std::endl;
    for (uint32_t i = PACKET_HEADER_SIZE; i < packet.getDataLength() + PACKET_HEADER_SIZE; ++i)
    {
        stream << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (uint16_t)packet.m_buffer[i] << " ";
        if (i % 16 == 0)
            stream << std::endl;
    }
    stream.flags(flags);

    return stream;
}

void Packet::setPacketSender(const boost::asio::ip::udp::endpoint& sender)
{
    m_sender = sender;
}

boost::asio::ip::udp::endpoint Packet::getPacketSender() const
{
    return m_sender;
}

template <typename T> void Packet::skip()
{
    m_readPos += sizeof(T);
}

Packet& Packet::operator<<(const int8_t& data)
{
    write<int8_t>(data);
    return *this;
}

Packet& Packet::operator<<(const int16_t& data)
{
    write<int16_t>(data);
    return *this;
}

Packet& Packet::operator<<(const int32_t& data)
{
    write<int32_t>(data);
    return *this;
}

Packet& Packet::operator<<(const int64_t& data)
{
    write<int64_t>(data);
    return *this;
}

Packet& Packet::operator<<(const uint8_t& data)
{
    write<uint8_t>(data);
    return *this;
}

Packet& Packet::operator<<(const uint16_t& data)
{
    write<uint16_t>(data);
    return *this;
}

Packet& Packet::operator<<(const uint32_t& data)
{
    write<uint32_t>(data);
    return *this;
}

Packet& Packet::operator<<(const uint64_t& data)
{
    write<uint64_t>(data);
    return *this;
}

Packet& Packet::operator<<(const bool& data)
{
    write<bool>(data);
    return *this;
}

Packet& Packet::operator<<(const char* data)
{
    writeString(data);
    return *this;
}

Packet& Packet::operator<<(const std::string& data)
{
    writeString(data);
    return *this;
}

Packet& Packet::operator>>(int8_t& data)
{
    data = read<int8_t>();
    return *this;
}

Packet& Packet::operator>>(int16_t& data)
{
    data = read<int16_t>();
    return *this;
}

Packet& Packet::operator>>(int32_t& data)
{
    data = read<int32_t>();
    return *this;
}

Packet& Packet::operator>>(int64_t& data)
{
    data = read<int64_t>();
    return *this;
}

Packet& Packet::operator>>(uint8_t& data)
{
    data = read<uint8_t>();
    return *this;
}

Packet& Packet::operator>>(uint16_t& data)
{
    data = read<uint16_t>();
    return *this;
}

Packet& Packet::operator>>(uint32_t& data)
{
    data = read<uint32_t>();
    return *this;
}

Packet& Packet::operator>>(uint64_t& data)
{
    data = read<uint64_t>();
    return *this;
}

Packet& Packet::operator>>(bool& data)
{
    data = read<bool>();
    return *this;
}

Packet& Packet::operator>>(std::string& data)
{
    readString(data);
    return *this;
}
