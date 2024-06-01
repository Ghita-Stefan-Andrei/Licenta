#include "ETH_Module.h"

void EthernetModule::updateLinkStatus(const EthernetLinkStatus& ethStatus)
{
    this->status = ethStatus;
}

void EthernetModule::setLastStatus(const EthernetLinkStatus& ethStatus)
{
    this->lastStatus = ethStatus;
}

bool EthernetModule::ethStatusChanged()
{
    return this->status != this->lastStatus;
}

EthernetLinkStatus EthernetModule::getStatus()
{
    return this->status;
}

void EthernetModule::handleEthernetConnectionChanges()
{
    switch(status)
    {
        case LinkOFF: digitalWrite(LED_RED, HIGH);  break;  // Light up the red LED when the connection is broken
        case LinkON : NVIC_SystemReset(); break;  // Reset the board to try to reconnect to the internet after an Ethernet connection is reestablished
        default:break;
    }
}