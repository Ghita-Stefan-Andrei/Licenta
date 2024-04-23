#pragma once
#include <cstdint>
#include <cstring>

#define START_BYTE    0xAA
#define RISING_SLOPE  0x01
#define FALLING_SLOPE 0x10
#define EXTRA_BYTES   (uint8_t)3
#define PACKET_OFFSET EXTRA_BYTES - 1
#define FIRST_BYTE    0
#define SECOND_BYTE   1

class Packet
{
  public:
    uint8_t  startByte;
    uint8_t* dataBytes;
    uint8_t  checkSum;
    uint8_t  dataSize;
    uint8_t* buildPacket;
    char*    builtPacket;
  public:
    Packet(const uint8_t* data, uint8_t length);
    ~Packet();
    void calculateCheckSum();
    char* sendPacket();
};