#pragma once
#include <Arduino.h>
#include <STM32Ethernet.h>

#define ETH_CONNECTION_LED   LED_RED
#define LOOP_FUNCTION_ACTIVE_LED LED_BLUE

class LedDriver
{
    private:
        static bool initialized;
    public:

        /**
          * @brief Constructor pentru clasa LedDriver.
          *
          * Inițializează obiectul LedDriver și setează flag-ul initialized la false.
          *
          * @return Nimic
          */
        LedDriver();

       /**
         * @brief Inițializează pinii LED pentru starea Ethernet și funcția loop.
         *
         * Această funcție setează modurile pinilor LED la OUTPUT. ETH_CONNECTION_LED
         * este folosit pentru a indica starea conexiunii la Ethernet. Dacă LED-ul este aprins, placa
         * nu este conectată la Ethernet. LOOP_FUNCTION_ACTIVE_LED este folosit pentru a indica
         * când placa intră în funcția loop.
         *
         * @return void
         */
        void init();
        
        /**
          * @brief Această funcție este folosită pentru a afișa starea conexiunii Ethernet pe LED.
          *
          * @param ethStatus Starea curentă a conexiunii Ethernet.
          *
          * @return void
          *
          * @details Această funcție primește starea conexiunii Ethernet ca intrare și
          *          controlează LED-ul corespunzător. Dacă starea conexiunii este LinkON,
          *          LED-ul de conexiune Ethernet este stins. Dacă starea conexiunii este
          *          LinkOFF, LED-ul de conexiune Ethernet este aprins. Dacă starea conexiunii
          *          nu este nici LinkON, nici LinkOFF, nu se întâmplă nimic.
          */
        static void displayEthStatus(const EthernetLinkStatus& ethStatus);

        /**
          * @brief Închide LED-ul conexiunii Ethernet.
          *
          * Această funcție setează pinul conectat la LED-ul de conexiune Ethernet la nivel scăzut,
          * efectiv stingându-l. Acest lucru indică că placa este conectată la
          * Ethernet.
          *
          * @return void
          *
          * @note Această funcție ar trebui apelată doar după ce LedDriver a fost
          * inițializat folosind metoda init().
          *
          * @see init()
          * @see openEthConLed()
          */
        static void closeEthConLed();

        /**
          * @brief Funcție pentru a porni LED-ul de conexiune Ethernet.
          *
          * Această funcție setează pinul conectat la LED-ul de conexiune Ethernet la nivel înalt,
          * indicând că placa nu este conectată la Ethernet.
          *
          * @return void
          *
          * @note Această funcție ar trebui apelată doar după ce obiectul LedDriver a fost inițializat.
          *
          * @see init()
          * @see closeEthConLed()
          */
        static void openEthConLed();

        /**
          * @brief Funcție pentru a indica faptul că placa a intrat în funcția loop.
          *
          * Această funcție setează pinul LOOP_FUNCTION_ACTIVE_LED la nivel înalt, indicând că placa se află în funcția loop.
          *
          * @return void
          */
        static void enteredLoopLed();
};