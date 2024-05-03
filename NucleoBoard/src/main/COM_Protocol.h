#pragma once
#include <cstdint>
#include <cstring>
#include "COM_Protocol_Defines.h"

class Packet
{
  private:
    BYTE  startByte;
    BYTE* dataBytes;
    BYTE  checkSum;
    BYTE  dataSize;
    BYTE* buildPacket;
    char* builtPacket;

  private:
    /**
      * @brief Creates a trigger packet with timestamp data and slope information.
      * 
      * This function initializes the data array for the packet, sets its size,
      * places a predefined type of trigger packet at the specified byte position,
      * copies the provided timestamp data starting from a specific byte position after the type byte,
      * and sets the slope byte as the last data byte of the packet.
      *
      * @param timeData Pointer to the array containing the timestamp data.
      * @param slopeByte The byte representing the slope information.
      */
    void createTriggerPacket(BYTE* dataByteArr, BYTE extraByte);

    /**
      * @brief Creates a boot packet.
      * 
      * This function allocates memory for the boot packet, sets its size,
      * and assigns the boot packet type to the type byte position.
      */
    void createBootPacket();

    /**
      * @brief Creates an Ethernet status packet with IP address and connection status.
      * 
      * This function initializes the data array for the Ethernet packet, sets its size,
      * assigns the Ethernet status packet type at the predefined position, copies the provided IP address
      * into the packet, and sets the Ethernet connection status as the last byte of the packet.
      *
      * @param ipAdress Pointer to the array containing the IP address.
      * @param ethStatus The Ethernet connection status (connected or disconnected).
      */
    void createEthernetPacket(BYTE* dataByteArr, BYTE extraByte, uint8_t type);

    /**
      * @brief Creates an NTP status packet to check if the internal time has been updated.
      * 
      * This function creates a packet with the information about the state of the time client,
      *
      * @param extraByte The extra byte containing the status information.
      */
    void createNTPstatusPacket(BYTE extraByte);

    /**
      * @brief Calculates the checksum for the packet.
      * 
      * Initializes the checksum with the value of startByte, then applies XOR between the current checksum 
      * and each data byte in the packet, updating the checksum.
      * 
      * This method updates the 'checkSum' member of the class.
      *
      * @note This method has no input parameters and does not return a value, it only updates the internal state of the Packet object.
      */
    void calculateCheckSum();

  public:
    /**
      * @brief Constructor for the Packet class.
      *
      * This constructor initializes a new Packet object with the specified data.
      * It sets the start byte to a default value, allocates and copies the received data
      * into an internal buffer, and calculates the checksum for the packet.
      *
      * @param data Pointer to the array of data to be included in the packet.
      * @param length Length of the data array, in bytes.
      *
      * @note Memory for `dataBytes` is dynamically allocated and is released in the destructor.
      */
    Packet(const BYTE* data, uint8_t length);
    
    /**
      * @brief Constructor for creating specific types of packets based on input parameters.
      * 
      * This constructor sets the start byte and initiates the creation of the packet based on
      * the provided packet type (e.g., trigger, boot, or Ethernet). It also calculates the checksum
      * for the created packet.
      *
      * @param packetType The type of packet to be created.
      * @param dataByteArr Pointer to an array of data bytes used for creating the packet (varies based on the packet type).
      * @param extraByte An additional byte used in some packet types to convey extra information (such as slope type for trigger packets, or Ethernet connection status for Ethernet packets).
      */
    Packet(const uint8_t packetType, BYTE* dataByteArr = nullptr, BYTE extraByte = 0x00);

    /**
      * @brief Builds and returns a data packet as a hexadecimal string.
      *
      * This function generates a data packet that includes a start byte, data size,
      * the actual data, and a checksum, all of which are then converted to hexadecimal format.
      * The generated packet is dynamically allocated and will be automatically released by the class destructor.
      *
      * @return A pointer to the constructed hexadecimal packet as a string of characters.
      *         This string is null-terminated for use as a C-string.
      */
    char* buildHexStringPacket();

    /**
      * @brief Destructor for the Packet class.
      *
      * This destructor releases the dynamically allocated memory for the 'dataBytes', 'buildPacket', and 'builtPacket' fields.
      * It is essential to ensure that the Packet object is destructed only after the packet has been sent and
      * is no longer needed, to avoid using invalid references to deallocated memory.
      */
    ~Packet();
};