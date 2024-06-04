#pragma once
#include <Arduino.h>
#include <STM32Ethernet.h>

#define ETH_CONNECTION_LED   LED_RED
#define LOOP_FUNCTION_ACTIVE_LED LED_BLUE

class LedDriver
{
    private:
        bool initialized;
    public:
        /**
          * @brief Constructor for LedDriver class.
          *
          * Initializes the LedDriver object and sets the initialized flag to false.
          *
          * @return None
          */
        LedDriver();

       /**
         * @brief Initializes the LED pins for the Ethernet and Loop Function status.
         *
         * This function sets the pin modes for the LED pins to OUTPUT. The ETH_CONNECTION_LED
         * is used to indicate the connection status to the Ethernet. If the LED is on, the board
         * is not connected to Ethernet. The LOOP_FUNCTION_ACTIVE_LED is used to indicate when
         * the board enters the loop function.
         *
         * @return void
         */
        void init();
        /**
          * @brief This function is used to display the Ethernet link status on the LED.
          *
          * @param ethStatus The current Ethernet link status.
          *
          * @return void
          *
          * @details This function takes the Ethernet link status as input and
          *          controls the LED accordingly. If the link status is LinkON,
          *          the Ethernet connection LED is turned off. If the link status
          *          is LinkOFF, the Ethernet connection LED is turned on.
          *          If the link status is neither LinkON nor LinkOFF, no action is taken.
          */
        static void displayEthStatus(const EthernetLinkStatus& ethStatus);

        /**
          * @brief Closes the Ethernet connection LED.
          *
          * This function sets the pin connected to the Ethernet connection LED to low,
          * effectively turning it off. This indicates that the board is connected to the
          * Ethernet.
          *
          * @return void
          *
          * @note This function should only be called after the LedDriver has been
          * initialized using the init() method.
          *
          * @see init()
          * @see openEthConLed()
          */
        static void closeEthConLed();

        /**
          * @brief Function to turn on the Ethernet connection LED.
          *
          * This function sets the pin connected to the Ethernet connection LED to HIGH,
          * indicating that the board is not connected to the Ethernet.
          *
          * @return void
          *
          * @note This function should be called only after the LedDriver object has been initialized.
          *
          * @see init()
          * @see closeEthConLed()
          */
        static void openEthConLed();

        /**
          * @brief Function to indicate that the board has entered the loop function.
          *
          * This function sets the LOOP_FUNCTION_ACTIVE_LED pin to HIGH, indicating that the board is in the loop function.
          *
          * @return void
          */
        static void enteredLoopLed();
};