#pragma once
#include "defines.h"
#include "COM_Protocol.h"
#include <NTPClient_Generic.h>

#define SIGNAL_MONITOR_PIN PC8
#define TIME_ZONE_OFFSET_HRS (3)

typedef struct
{
  bool status;
  uint32_t ip;
}ethInfo;

ethInfo initEthernet()
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
  
  bool status = Ethernet.begin(mac[index]);

  ethInfo toRet;
  toRet.status = status;
  toRet.ip = Ethernet.localIP();
  
  return toRet;
}

void getTimeStampAsByteArray(NTPClient* timeClient, uint8_t* data, uint32_t updatedMillis)
{
  uint16_t milisecs = updatedMillis % 1000;

  data[0] = timeClient->getYear() - 2000; //to get a value to fit in an uint8_t assuming this code wont be used after the year 2255
  data[1] = timeClient->getMonth();
  data[2] = timeClient->getDay();
  data[3] = timeClient->getHours();
  data[4] = timeClient->getMinutes();
  data[5] = timeClient->getSeconds();
  data[6] = milisecs / 100;
  data[7] = milisecs % 100; 
}

void decodeIP(uint8_t* IP, uint32_t rawIP)
{
  IP[0] = (rawIP >>  0) & 0x000000FF;
  IP[1] = (rawIP >>  8) & 0x000000FF;
  IP[2] = (rawIP >> 16) & 0x000000FF;
  IP[3] = (rawIP >> 24) & 0x000000FF;
}

uint32_t performNtpRequestAndCalibrateMlisecs(NTPClient &timeClient) 
{
  uint32_t beforeNtpRequest = millis();
  timeClient.update();
  uint32_t afterNtpRequest = millis();

  uint32_t serverResponseTime = afterNtpRequest - beforeNtpRequest;
  return timeClient.getEpochMillis() - serverResponseTime;
}