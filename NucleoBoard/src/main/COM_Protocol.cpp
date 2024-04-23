#include "COM_Protocol.h"

Packet::Packet(const uint8_t* data, uint8_t length)
{
    this->startByte = START_BYTE;
    this->dataBytes = new uint8_t[length];
    this->dataSize = length;
    memcpy(this->dataBytes, data, length);
    this->calculateCheckSum();
}

void Packet::calculateCheckSum()
{
    this->checkSum = this->startByte;
    for (uint16_t dataByteIndex = 0; dataByteIndex < this->dataSize; dataByteIndex++) 
    {
        this->checkSum ^= this->dataBytes[dataByteIndex];  
    }
}

char* Packet::sendPacket()
{
	const char hexDigits[] = "0123456789ABCDEF";

    uint8_t buildPacketSize = this->dataSize + EXTRA_BYTES;
    this->buildPacket = new uint8_t[buildPacketSize];

    this->buildPacket[FIRST_BYTE] = this->startByte;
    this->buildPacket[SECOND_BYTE] = this->dataSize;

    for (uint8_t dataByteIndex = 0; dataByteIndex < this->dataSize; dataByteIndex++)
    {
        this->buildPacket[dataByteIndex + PACKET_OFFSET] = this->dataBytes[dataByteIndex];
    }

    this->buildPacket[this->dataSize + PACKET_OFFSET] = this->checkSum;
    uint8_t finalPacketSize = 2 * buildPacketSize + 1;
    this->builtPacket = new char[finalPacketSize];

    for(uint8_t dataTransIndex = 0; dataTransIndex < buildPacketSize; dataTransIndex++)
    {
        this->builtPacket[2 * dataTransIndex] = hexDigits[(this->buildPacket[dataTransIndex] >> 4) & 0x0F];
        this->builtPacket[2 * dataTransIndex + 1] = hexDigits[(this->buildPacket[dataTransIndex]) & 0x0F];
    }
    this->builtPacket[finalPacketSize - 1] = '\0';

    return this->builtPacket;
}

Packet::~Packet() 
{
    delete[] this->dataBytes; 
    delete[] this->buildPacket;
    delete[] this-> builtPacket;
}