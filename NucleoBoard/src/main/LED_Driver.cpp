#include "LED_Driver.h"

bool LedDriver::initialized = false;
LedDriver::LedDriver()
{
    initialized = false;
}
void LedDriver::init()
{
    pinMode(ETH_CONNECTION_LED,  OUTPUT);    
    pinMode(LOOP_FUNCTION_ACTIVE_LED, OUTPUT);    
    initialized = true;
}

void LedDriver::displayEthStatus(const EthernetLinkStatus& ethStatus)
{   if(initialized)
    switch(ethStatus)
    {
        case LinkON: closeEthConLed(); break;
        case LinkOFF: openEthConLed(); break;
        default: break;
    }
}

void LedDriver::closeEthConLed()
{
    if(initialized)
    digitalWrite(ETH_CONNECTION_LED, LOW);
}

void LedDriver::openEthConLed()
{
    if(initialized)
    digitalWrite(ETH_CONNECTION_LED, HIGH);
}

void LedDriver::enteredLoopLed()
{
    if(initialized)
    digitalWrite(LOOP_FUNCTION_ACTIVE_LED, HIGH);
}