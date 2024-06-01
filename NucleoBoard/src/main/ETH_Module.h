#pragma once
#include <STM32Ethernet.h>
#include "LED_Driver.h"
#include "COM_Protocol_Defines.h"

#define ISOLATE_LAST_BYTE 0x000000FF

typedef struct
{
    bool status;
    uint32_t ip;
}ethInfo;

class EthernetModule
{
    public:
        EthernetLinkStatus status;
        EthernetLinkStatus lastStatus;
        BYTE* ip;
    public:
        /**
          * @brief Constructor for EthernetModule class.
          *
          * Initializes the IP address array with a new dynamic memory allocation.
          * The size of the array is defined by the constant IP_BYTE_LENGTH.
          *
          * @return None
          */
        EthernetModule();

        /**
          * @brief Destructor for EthernetModule class.
          *
          * This destructor is responsible for freeing the dynamically allocated memory for the IP address.
          * It is called when an object of EthernetModule class goes out of scope or is explicitly deleted.
          */
        ~EthernetModule();

        /**
          * @brief Updates the current Ethernet link status.
          *
          * This function updates the internal status variable with the provided EthernetLinkStatus.
          *
          * @param ethStatus The new Ethernet link status to be set.
          *
          * @return void
          */
        void updateLinkStatus(const EthernetLinkStatus& ethStatus);

        /**
          * @brief Sets the last recorded Ethernet link status.
          *
          * This function updates the last recorded Ethernet link status with the provided status.
          *
          * @param ethStatus The new Ethernet link status to be recorded.
          *
          * @return void
          */
        void setLastStatus(const EthernetLinkStatus& ethStatus);

        /**
          * @brief Handles the actions to be taken when the Ethernet connection status changes.
          *
          * This function checks the current Ethernet link status and performs specific actions based on the status.
          * If the link status is LinkOFF, it turns on the red LED.
          * If the link status is LinkON, it triggers a system reset to try to reconnect to the internet.
          *
          * @return void
          */
        void handleEthernetConnectionChanges();

        /**
          * @brief Checks if the Ethernet status has changed since the last update.
          *
          * @return True if the Ethernet status has changed, false otherwise.
          *
          * @note This function compares the current Ethernet status with the last recorded status.
          * It is used to determine if any changes have occurred in the Ethernet connection.
          */
        bool ethStatusChanged();

        /**
          * @brief Get the current Ethernet link status.
          *
          * This function returns the current Ethernet link status.
          *
          * @return EthernetLinkStatus The current Ethernet link status.
          */
        EthernetLinkStatus getStatus();

        /**
        * @brief Get the last recorded Ethernet link status.
        *
        * This function returns the last recorded Ethernet link status.
        *
        * @return EthernetLinkStatus The last recorded Ethernet link status.
        */
        EthernetLinkStatus getLastStatus();

        /**
          * @brief Decodes a 32-bit raw IP address into an array of 4 bytes.
          *
          * This function takes a 32-bit raw IP address and decodes it into an array of 4 bytes.
          * The raw IP address is assumed to be in network byte order (big-endian).
          *
          * @param rawIP The 32-bit raw IP address to decode.
          *
          * @return void. The decoded IP address is stored in the 'ip' member variable of the EthernetModule object.
          *
          * @note The 'ip' member variable is assumed to be a dynamically allocated array of 4 bytes.
          *
          * @warning This function does not perform any error checking or validation on the input 'rawIP'.
          *          It assumes that the input is a valid 32-bit raw IP address.
          */
        void decodeIP(uint32_t rawIP);

        /**
          * @brief Get the IP address of the Ethernet module.
          *
          * This function returns a pointer to the array that holds the IP address.
          * The IP address is stored in network byte order (big endian).
          *
          * @return BYTE* Pointer to the array that holds the IP address.
          *
          * @note The returned pointer points to the internal data of the EthernetModule object.
          *       Do not modify or delete the memory pointed by the returned pointer.
          *
          * @note The IP address is updated by the decodeIP function.
          *
          * @note The IP address is stored in the following format:
          *       ip[0] = first byte of the IP address
          *       ip[1] = second byte of the IP address
          *       ip[2] = third byte of the IP address
          *       ip[3] = fourth byte of the IP address
          */
        BYTE* getIPAddress();
};