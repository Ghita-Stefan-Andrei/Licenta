#pragma once
#include <Arduino.h>
#include <STM32Ethernet.h>

#define ETH_CONNECTION_LED   LED_RED
#define LOOP_FUNCTION_ACTIVE_LED LED_BLUE

class LedDriver
{
    private:

    public:
        void init();
        static void displayEthStatus(const EthernetLinkStatus& ethStatus);
        static void closeEthConLed();
        static void openEthConLed();
        static void enteredLoopLed();
};