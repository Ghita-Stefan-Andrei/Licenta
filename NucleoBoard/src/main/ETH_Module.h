#pragma once
#include <STM32Ethernet.h>

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
    public:
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
};