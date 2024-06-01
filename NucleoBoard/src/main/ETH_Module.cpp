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

EthernetLinkStatus EthernetModule::getLastStatus()
{
    return this->lastStatus;
}

void EthernetModule::handleEthernetConnectionChanges()
{
    switch(status)
    {
        case LinkOFF: LedDriver::openEtcConLed();  break;  // Light up the red LED when the connection is broken
        case LinkON : NVIC_SystemReset(); break;  // Reset the board to try to reconnect to the internet after an Ethernet connection is reestablished
        default:break;
    }
}

EthernetModule::EthernetModule()
{
    this->ip = new BYTE[IP_BYTE_LENGTH];
}

EthernetModule::~EthernetModule()
{
    delete[] this->ip;
}

void EthernetModule::decodeIP(uint32_t rawIP)
{
  this->ip[0] = (rawIP >>  0) & ISOLATE_LAST_BYTE;
  this->ip[1] = (rawIP >>  8) & ISOLATE_LAST_BYTE;
  this->ip[2] = (rawIP >> 16) & ISOLATE_LAST_BYTE;
  this->ip[3] = (rawIP >> 24) & ISOLATE_LAST_BYTE;
}

BYTE* EthernetModule::getIPAddress()
{
    return this->ip;
}