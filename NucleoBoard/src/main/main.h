#pragma once
#include "defines.h"
#include <NTPClient_Generic.h>
#include "COM_Protocol.h"

#define SIGNAL_MONITOR_PIN PC8
#define TIME_ZONE_OFFSET_HRS (3)

void initEthernet()
{
#if !(USE_BUILTIN_ETHERNET || USE_UIP_ETHERNET)

  ET_LOGWARN3(F("Board :"), BOARD_NAME, F(", setCsPin:"), USE_THIS_SS_PIN);

  ET_LOGWARN(F("Default SPI pinout:"));
  ET_LOGWARN1(F("MOSI:"), MOSI);
  ET_LOGWARN1(F("MISO:"), MISO);
  ET_LOGWARN1(F("SCK:"),  SCK);
  ET_LOGWARN1(F("SS:"),   SS);
  ET_LOGWARN(F("========================="));

  // For other boards, to change if necessary
  #if ( USE_ETHERNET_GENERIC || USE_ETHERNET_ENC )
    // Must use library patch for Ethernet, Ethernet2, EthernetLarge libraries
    Ethernet.init (USE_THIS_SS_PIN);
   
  #elif USE_CUSTOM_ETHERNET
    // You have to add initialization for your Custom Ethernet here
    // This is just an example to setCSPin to USE_THIS_SS_PIN, and can be not correct and enough
    //Ethernet.init(USE_THIS_SS_PIN);
  
  #endif  //( ( USE_ETHERNET_GENERIC || USE_ETHERNET_ENC )
#endif

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  Ethernet.begin(mac[index]);

  // you're connected now, so print out the data
  //Serial.print(F("You're connected to the network, IP = "));
  //Serial.println(Ethernet.localIP());  
}

void getTimeStampAsByteArray(NTPClient* timeClient, uint8_t* data)
{
  uint16_t milisecs = timeClient->getEpochMillis() % 1000;

  data[BYTES_BEFORE_TIME_DATA + 0] = timeClient->getYear() - 2000; //to get a value to fit in an uint8_t assuming this code wont be used after the year 2255
  data[BYTES_BEFORE_TIME_DATA + 1] = timeClient->getMonth();
  data[BYTES_BEFORE_TIME_DATA + 2] = timeClient->getDay();
  data[BYTES_BEFORE_TIME_DATA + 3] = timeClient->getHours();
  data[BYTES_BEFORE_TIME_DATA + 4] = timeClient->getMinutes();
  data[BYTES_BEFORE_TIME_DATA + 5] = timeClient->getSeconds();
  data[BYTES_BEFORE_TIME_DATA + 6] = milisecs / 100;
  data[BYTES_BEFORE_TIME_DATA + 7] = milisecs % 100; 
}