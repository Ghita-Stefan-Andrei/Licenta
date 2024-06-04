#pragma once
#include "defines.h"
#include "COM_Protocol.h"
#include "ETH_Module.h"
#include <NTPClient_Generic.h>

#define SIGNAL_MONITOR_PIN D13
#define TIME_ZONE_OFFSET_HRS (3)
#define BIT_RATE 115200
#define SERIAL_OPEN_TIMEOUT 5000
#define SECONDS_IN_HOUR 3600

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool lastPinState = LOW;
bool reconected = false;
ethInfo ethInfoStatus;
EthernetLinkStatus lastEthStatus;
EthernetModule ethModule;
LedDriver ledDriver();

/**
 * @brief Initializes the Ethernet connection and retrieves the IP address status.
 * 
 * @return An ethInfo structure containing the status of the Ethernet connection and the local IP address.
 * 
 * @details This function initializes the Ethernet connection using a random MAC address and DHCP dynamic IP. 
 * It retrieves the status of the Ethernet connection and the local IP address, and returns this information encapsulated in an ethInfo structure.
 */
ethInfo initEthernet()
{
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
void getTimeStampAsByteArray(NTPClient* timeClient, uint8_t* data, uint32_t responseTime)
{
  uint16_t milisecs = (timeClient->getEpochMillis() - responseTime) % 1000;

  data[5] = timeClient->getSeconds();
  data[4] = timeClient->getMinutes();
  data[3] = timeClient->getHours();
  data[2] = timeClient->getDay();
  data[1] = timeClient->getMonth();
  data[0] = timeClient->getYear() - 2000; // to get a value to fit in an uint8_t assuming this code won't be used after the year 2255
  data[6] = milisecs / 100;
  data[7] = milisecs % 100; 
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
uint32_t performNtpRequestAndGetResponseTime(NTPClient &timeClient) 
{
  uint32_t beforeNtpRequest = millis();
  timeClient.update();
  uint32_t afterNtpRequest = millis();

  uint32_t serverResponseTime = afterNtpRequest - beforeNtpRequest;
  return serverResponseTime;
}