#include "COM_Protocol.h"

Packet::Packet(const uint8_t* data, uint8_t length)
{
    this->startByte = START_BYTE;
    this->dataBytes = new uint8_t[length];
    this->dataSize = length;
    memcpy(this->dataBytes, data, length);
    this->calculateCheckSum();
}

void Packet::createTriggerPacket(uint8_t* timeData, uint8_t slopeByte)
{
    this->dataBytes = new uint8_t[DATA_BYTE_LENGTH_TRIG];
    this->dataSize = DATA_BYTE_LENGTH_TRIG;
    this->dataBytes[TYPE_BYTE_POSITION] = TRIGGER_TYPE;
    for (uint8_t timeDataIndex = 0; timeDataIndex < TIME_TO_BYTE_ARRAY_LEN; timeDataIndex++)
    {
        this->dataBytes[BYTES_BEFORE_TIME_DATA + timeDataIndex] = timeData[timeDataIndex];
    }
    this->dataBytes[SLOPE_BYTE_POSITION] = slopeByte;
}

void Packet::createBootPacket()
{
    this->dataBytes = new uint8_t[TYPE_BYTE];
    this->dataSize = TYPE_BYTE;
    this->dataBytes[TYPE_BYTE_POSITION] = BOOT_TYPE;
}

void Packet::createEthernetPacket(uint8_t* ipAdress, uint8_t ethStatus)
{
    this->dataBytes = new uint8_t[DATA_BYTE_LENGTH_ETH];
    this->dataSize = DATA_BYTE_LENGTH_ETH;
    this->dataBytes[TYPE_BYTE_POSITION] = ETHERNET_STATUS_TYPE;
    for (uint8_t ipByte = 0; ipByte < IP_BYTE_LENGTH; ipByte++)
    {
        this->dataBytes[BYTES_BEFORE_IP + ipByte] = ipAdress[ipByte];
    }
    this->dataBytes[ETH_STATUS_BYTE_POS] = (ethStatus == ETH_CONNECTED) ? ETH_CONNECTED : ETH_NOT_CONNECTED;
}

Packet::Packet(const uint8_t packetType, uint8_t* dataByteArr, uint8_t extraByte)
{
    this->startByte = START_BYTE;
    switch(packetType)
    {
        case TRIGGER_TYPE:         createTriggerPacket(dataByteArr, extraByte); break;
        case BOOT_TYPE:            createBootPacket(); break;
        case ETHERNET_STATUS_TYPE: createEthernetPacket(dataByteArr, extraByte); break;
    }
    this->calculateCheckSum();
}

void Packet::calculateCheckSum()
{
    this->checkSum = this->startByte ^ this->dataSize;
    for (uint16_t dataByteIndex = 0; dataByteIndex < this->dataSize; dataByteIndex++) 
    {
        this->checkSum ^= this->dataBytes[dataByteIndex];  
    }
}

char* Packet::buildHexStringPacket()
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
    delete[] this->builtPacket; //!warning: Object should be deconstructed after the packet was sent.
}