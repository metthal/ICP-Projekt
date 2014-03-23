#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <vector>
#include <string>

#define PACKET_HEADER_OPCODE_POS    0
#define PACKET_HEADER_LENGTH_POS    1
#define PACKET_HEADER_SIZE          3

class Packet
{
public:
    Packet() = delete;

    Packet(uint8_t opcode, uint16_t length);
    Packet(const std::vector<uint8_t>& buffer, uint32_t count);
    Packet(const Packet& packet);

    ~Packet();

    Packet& operator=(const Packet& packet);

    std::vector<uint8_t> getBufferCopy() const;
    uint8_t getOpcode() const;
    uint16_t getDataLength() const;
    uint32_t getBufferSize() const;

    Packet& operator<<(const int8_t& data);
    Packet& operator<<(const int16_t& data);
    Packet& operator<<(const int32_t& data);
    Packet& operator<<(const int64_t& data);
    Packet& operator<<(const uint8_t& data);
    Packet& operator<<(const uint16_t& data);
    Packet& operator<<(const uint32_t& data);
    Packet& operator<<(const uint64_t& data);
    Packet& operator<<(const bool& data);
    Packet& operator<<(const std::string& data);

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

    template <typename T> void skip();

private:
    template <typename T> void write(const T& data);
    template <typename T> T read();
    void writeString(const std::string& str);
    void readString(std::string& str);

    uint32_t m_writePos, m_readPos;
    std::vector<uint8_t> m_buffer;
};

#endif // PACKET_H
