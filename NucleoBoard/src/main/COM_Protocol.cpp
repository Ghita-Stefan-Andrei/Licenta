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

    //copy timeData into the dataBytes array at the position marked by the offset
    memcpy(this->dataBytes + BYTES_BEFORE_TIME_DATA, timeData, TIME_DATA_BYTE_COUNT);

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

        //copy the IP adress in the dataBytes array at the position marked by the offset
        memcpy(this->dataBytes + BYTES_BEFORE_IP, ipAdress, IP_BYTE_LENGTH);

        this->dataBytes[ETH_STATUS_BYTE_POS] = (ethStatus == ETH_CONNECTED) ? ETH_CONNECTED : ETH_NOT_CONNECTED;
    }
    else if (type == ETHERNET_STATUS_CHECK_T)
    {
        this->dataBytes = new BYTE[DATA_BYTE_LENGTH_ETH_C];
        this->dataSize = DATA_BYTE_LENGTH_ETH_C;
        this->dataBytes[TYPE_BYTE_POSITION] = ETHERNET_STATUS_CHECK_T;
        switch(ethStatus)
        {
            case ETH_CONNECTED:      this->dataBytes[ETH_STATUS_BYTE_POS_C] = ETH_CONNECTED; break;
            case ETH_CONNECTION_OFF: this->dataBytes[ETH_STATUS_BYTE_POS_C] = ETH_NOT_CONNECTED; break;
            default:                 this->dataBytes[ETH_STATUS_BYTE_POS_C] = ETH_CONNECTION_UNKNOWN; break;
        }
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
    //set the Start byte to 0xAA
    this->startByte = START_BYTE;

    //select the structure of the packet based on its type
    switch(packetType)
    {
        case TRIGGER_TYPE:            createTriggerPacket(dataByteArr, extraByte); break;
        case BOOT_TYPE:               createBootPacket(); break;
        case ETHERNET_STATUS_TYPE:    createEthernetPacket(dataByteArr, extraByte, packetType); break;
        case ETHERNET_STATUS_CHECK_T: createEthernetPacket(dataByteArr, extraByte, packetType); break;
        case NTP_STATUS_TYPE:         createNTPstatusPacket(extraByte); break;
        default: break;
    }

    //calculate check sum
    this->checkSum = this->calculateCheckSum();
}

BYTE Packet::calculateCheckSum()
{
    BYTE checkSumCalc = this->startByte ^ this->dataSize;

    for (uint16_t dataByteIndex = 0; dataByteIndex < this->dataSize; dataByteIndex++) 
        checkSumCalc ^= this->dataBytes[dataByteIndex];  

    return checkSumCalc;
}

char* Packet::buildHexStringPacket()
{
    //array to be used to convert bytes from int to char 
    const char hexDigits[] = "0123456789ABCDEF";

    uint8_t buildPacketSize = this->dataSize + NON_DATA_BYTE_COUNT;
    this->buildPacket = new BYTE[buildPacketSize];

    //set the first byte of the pack to be the start byte and the second byte as the length of the data payload
    this->buildPacket[START_BYTE_POSITION] = this->startByte;
    this->buildPacket[LENGTH_BYTE_POSITION] = this->dataSize;

    //copy the data bytes in the packet at the position marked by the offset
    memcpy(this->buildPacket + PACKET_HEADER_SIZE, this->dataBytes, this->dataSize);

    //add the checksum byte to the packet
    this->buildPacket[this->dataSize + PACKET_HEADER_SIZE] = this->checkSum;

    //create a new version of the packet to convert the uint8_t to char
    uint8_t finalPacketSize = 2 * buildPacketSize + 1;
    this->builtPacket = new char[finalPacketSize];

    for(uint8_t dataTransIndex = 0; dataTransIndex < buildPacketSize; dataTransIndex++)
    {
        //split the bytes from a uint8_t in 2 chars by isolating the first 4 bits and the last 4 bits
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
    delete[] this->builtPacket;
}