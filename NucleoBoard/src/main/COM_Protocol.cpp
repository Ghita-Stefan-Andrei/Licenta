#include "COM_Protocol.h"

Packet::Packet(const BYTE* data, uint8_t length)
{
    this->startByte = START_BYTE;
    this->dataBytes = new BYTE[length];
    this->dataSize = length;
    memcpy(this->dataBytes, data, length);
    this->calculateCheckSum();
}

void Packet::createTriggerPacket(BYTE* timeData, BYTE slopeType)
{
    this->dataBytes = new BYTE[DATA_BYTE_LENGTH_TRIG];
    this->dataSize = DATA_BYTE_LENGTH_TRIG;
    this->dataBytes[TYPE_BYTE_POSITION] = TRIGGER_TYPE;

    for (uint8_t timeDataIndex = 0; timeDataIndex < TIME_TO_BYTE_ARRAY_LEN; timeDataIndex++)
    {
        this->dataBytes[BYTES_BEFORE_TIME_DATA + timeDataIndex] = timeData[timeDataIndex];
    }

    this->dataBytes[SLOPE_BYTE_POSITION] = (slopeType == HIGH) ? RISING_SLOPE : FALLING_SLOPE;
}

void Packet::createBootPacket()
{
    this->dataBytes = new BYTE[TYPE_BYTE];
    this->dataSize = TYPE_BYTE;
    this->dataBytes[TYPE_BYTE_POSITION] = BOOT_TYPE;
}

void Packet::createEthernetPacket(BYTE* ipAdress, BYTE ethStatus, uint8_t type)
{
    if(type == ETHERNET_STATUS_TYPE)
    {
        this->dataBytes = new BYTE[DATA_BYTE_LENGTH_ETH];
        this->dataSize = DATA_BYTE_LENGTH_ETH;
        this->dataBytes[TYPE_BYTE_POSITION] = ETHERNET_STATUS_TYPE;

        for (uint8_t ipByte = 0; ipByte < IP_BYTE_LENGTH; ipByte++)
        {
            this->dataBytes[BYTES_BEFORE_IP + ipByte] = ipAdress[ipByte];
        }

        this->dataBytes[ETH_STATUS_BYTE_POS] = (ethStatus == ETH_CONNECTED) ? ETH_CONNECTED : ETH_NOT_CONNECTED;
    }
    else if (type == ETHERNET_STATUS_CHECK_T)
    {
        this->dataBytes = new BYTE[DATA_BYTE_LENGTH_ETH_C];
        this->dataSize = DATA_BYTE_LENGTH_ETH_C;
        this->dataBytes[TYPE_BYTE_POSITION] = ETHERNET_STATUS_CHECK_T;
        this->dataBytes[ETH_STATUS_BYTE_POS_C] = (ethStatus == ETH_CONNECTED) ? ETH_CONNECTED : (ethStatus == ETH_CONNECTION_OFF) ? ETH_NOT_CONNECTED : ETH_CONNECTION_UNKNOWN;
    }
}

void Packet::createNTPstatusPacket(BYTE ntpStatus)
{
    this->dataBytes = new BYTE[DATA_BYTE_LENGTH_NTP_S];
    this->dataSize = DATA_BYTE_LENGTH_NTP_S;
    this->dataBytes[TYPE_BYTE_POSITION] = NTP_STATUS_TYPE;
    this->dataBytes[NTP_STATUS_POSITION] = ntpStatus;
}

Packet::Packet(const uint8_t packetType, BYTE* dataByteArr, BYTE extraByte)
{
    this->startByte = START_BYTE;
    switch(packetType)
    {
        case TRIGGER_TYPE:            createTriggerPacket(dataByteArr, extraByte); break;
        case BOOT_TYPE:               createBootPacket(); break;
        case ETHERNET_STATUS_TYPE:    createEthernetPacket(dataByteArr, extraByte, packetType); break;
        case ETHERNET_STATUS_CHECK_T: createEthernetPacket(dataByteArr, extraByte, packetType); break;
        case NTP_STATUS_TYPE:         createNTPstatusPacket(extraByte); break;
        default: break;
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
  this->buildPacket = new BYTE[buildPacketSize];

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