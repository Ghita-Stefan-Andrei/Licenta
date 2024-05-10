#pragma once
#include "defines.h"
#include "COM_Protocol.h"
#include <NTPClient_Generic.h>

#define SIGNAL_MONITOR_PIN PC8
#define ISOLATE_LAST_BYTE 0x000000FF
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

/**
 * @brief Converts the timestamp obtained from the NTPClient into a byte array.
 * 
 * @param timeClient Pointer to the NTPClient object providing the timestamp information.
 * @param data Pointer to the array where the timestamp will be stored.
 * @param updatedMillis The updated milliseconds value used to calculate the timestamp.
 * 
 * @details This function extracts the year, month, day, hours, minutes, seconds, and milliseconds from the NTPClient object 
 * and stores them in the provided byte array. The year is offset by 2000 to fit within an uint8_t, assuming the code won't be 
 * used after the year 2255.
 */
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

/**
 * @brief Decodes the raw IP address into an array of bytes.
 * 
 * @param IP Pointer to the array where the decoded IP address will be stored.
 * @param rawIP The raw IP address to be decoded.
 * 
 * @details This function takes a 32-bit raw IP address and decodes it into an array of bytes. 
 * It extracts the individual bytes from the raw IP address and stores them in the provided byte array.
 */
void decodeIP(uint8_t* IP, uint32_t rawIP)
{
  IP[0] = (rawIP >>  0) & ISOLATE_LAST_BYTE;
  IP[1] = (rawIP >>  8) & ISOLATE_LAST_BYTE;
  IP[2] = (rawIP >> 16) & ISOLATE_LAST_BYTE;
  IP[3] = (rawIP >> 24) & ISOLATE_LAST_BYTE;
}

/**
 * @brief Performs an NTP request and calibrates the milliseconds value.
 * 
 * @param timeClient Reference to the NTPClient object used to perform the NTP request.
 * 
 * @return The calibrated milliseconds value obtained by subtracting the server response time from the epoch milliseconds obtained from the NTPClient object.
 * 
 * @details This function captures the current milliseconds value before sending an NTP request using the provided NTPClient object. 
 * It then updates the timeClient to obtain the server response time and captures the milliseconds value after the NTP request. 
 * The server response time is calculated as the difference between the two captured milliseconds values. 
 * Finally, the calibrated milliseconds value is obtained by subtracting the server response time from the epoch milliseconds obtained from the NTPClient object, and it is returned.
 */
uint32_t performNtpRequestAndCalibrateMlisecs(NTPClient &timeClient) 
{
  uint32_t beforeNtpRequest = millis();
  timeClient.update();
  uint32_t afterNtpRequest = millis();

  uint32_t serverResponseTime = afterNtpRequest - beforeNtpRequest;
  return timeClient.getEpochMillis() - serverResponseTime;
}