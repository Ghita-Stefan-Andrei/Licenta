#include "LED_Driver.h"
void LedDriver::init()
{
    pinMode(ETH_CONNECTION_LED,  OUTPUT);    //the RED led marks the connection to ethernet. If it is on, the board is not connected to ethernet.
    pinMode(LOOP_FUNCTION_ACTIVE_LED, OUTPUT);    //the BLUE led marks when the board enters the loop function
}

void LedDriver::displayEthStatus(const EthernetLinkStatus& ethStatus)
{
    switch(ethStatus)
    {
        case LinkON: closeEtcConLed(); break;
        case LinkOFF: openEtcConLed(); break;
        default: break;
    }
}

void LedDriver::closeEtcConLed()
{
    digitalWrite(ETH_CONNECTION_LED, LOW);
}

void LedDriver::openEtcConLed()
{
    digitalWrite(ETH_CONNECTION_LED, HIGH);
}

void LedDriver::enteredLoopLed()
{
    digitalWrite(LOOP_FUNCTION_ACTIVE_LED, HIGH);
}