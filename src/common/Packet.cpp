#include <cstring>
#include "common/Packet.h"

Packet::Packet(uint8_t opcode, uint16_t length) : m_writePos(PACKET_HEADER_SIZE), m_readPos(PACKET_HEADER_SIZE)
{
    m_buffer.resize(PACKET_HEADER_SIZE + length, 0);
    m_buffer[PACKET_HEADER_OPCODE_POS] = opcode;
    *((uint16_t*)&m_buffer[PACKET_HEADER_LENGTH_POS]) = length;
}

Packet::Packet(const std::vector<uint8_t>& buffer, uint32_t count) : m_writePos(count), m_readPos(PACKET_HEADER_SIZE), m_buffer(buffer.begin(), buffer.begin() + count)
{
}

Packet::Packet(const Packet& packet) : m_writePos(packet.m_writePos), m_readPos(packet.m_readPos),  m_buffer(packet.m_buffer.begin(), packet.m_buffer.end())
{
}

Packet::~Packet() { }

Packet& Packet::operator=(const Packet& packet)
{
    m_writePos = packet.m_writePos;
    m_readPos = packet.m_readPos;
    m_buffer = std::vector<uint8_t>(packet.m_buffer.begin(), packet.m_buffer.end());
    return *this;
}

std::vector<uint8_t> Packet::getBufferCopy() const
{
    return m_buffer;
}

uint8_t Packet::getOpcode() const
{
    return m_buffer[PACKET_HEADER_OPCODE_POS];
}

uint16_t Packet::getDataLength() const
{
    return *((uint16_t*)&m_buffer[PACKET_HEADER_LENGTH_POS]);
}

uint32_t Packet::getBufferSize() const
{
    return m_buffer.size();
}

template <typename T> void Packet::write(const T& data)
{
    size_t dataSize = sizeof(T);
    memcpy(&m_buffer[m_writePos], &data, dataSize);
    m_writePos += dataSize;
}

void Packet::writeString(const std::string& str)
{
    // write only maximum 255 characters
    size_t strLen = str.length();
    if (strLen > 255)
        strLen = 255;

    // write the length of the string before the string itself
    write<uint8_t>(strLen);

    memcpy(&m_buffer[m_writePos], str.c_str(), strLen);
    m_writePos += strLen;
}

template <typename T> T Packet::read()
{
    T& data = *((T*)&m_buffer[m_readPos]);
    m_readPos += sizeof(T);
    return data;
}

void Packet::readString(std::string& str)
{
    uint8_t strLen = read<uint8_t>();
    str = std::string(m_buffer.begin() + m_readPos, m_buffer.begin() + m_readPos + strLen);
    m_readPos += strLen;
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
