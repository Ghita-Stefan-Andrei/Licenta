#include "LED_Driver.h"

LedDriver::LedDriver()
{
    this->initialized = false;
}
void LedDriver::init()
{
    pinMode(ETH_CONNECTION_LED,  OUTPUT);    //the RED led marks the connection to ethernet. If it is on, the board is not connected to ethernet.
    pinMode(LOOP_FUNCTION_ACTIVE_LED, OUTPUT);    //the BLUE led marks when the board enters the loop function
    this->initialized = true;
}

void LedDriver::displayEthStatus(const EthernetLinkStatus& ethStatus)
{
    if (this->initialized)
        switch(ethStatus)
        {
            case LinkON: closeEthConLed(); break;
            case LinkOFF: openEthConLed(); break;
            default: break;
        }
}

void LedDriver::closeEthConLed()
{
    if (this->initialized) digitalWrite(ETH_CONNECTION_LED, LOW);
}

void LedDriver::openEthConLed()
{
    if (this->initialized) digitalWrite(ETH_CONNECTION_LED, HIGH);
}

void LedDriver::enteredLoopLed()
{
    if (this->initialized) digitalWrite(LOOP_FUNCTION_ACTIVE_LED, HIGH);
}