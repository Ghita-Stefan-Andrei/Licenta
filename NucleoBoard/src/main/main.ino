#include "main.h"

void setup() {
  ledDriver.init();
   
  Serial.begin(BIT_RATE);
  while (!Serial && millis() < SERIAL_OPEN_TIMEOUT);
  
  Packet bootPack(BOOT_TYPE);
  Serial.print(bootPack.buildHexStringPacket());

  // Ethernet
  ethInfoStatus = initEthernet();

  ethModule.decodeIP(ethInfoStatus.ip);

  Packet ethPack(ETHERNET_STATUS_TYPE, ethModule.getIPAddress(), ethInfoStatus.status);
  Serial.print(ethPack.buildHexStringPacket());

  ethModule.setLastStatus(Ethernet.linkStatus());

  ledDriver.displayEthStatus(ethModule.getLastStatus());

  // End Ethernet //
  timeClient.begin();
  timeClient.setTimeOffset(SECONDS_IN_HOUR * TIME_ZONE_OFFSET_HRS);
  timeClient.setUpdateInterval(SECS_IN_HR); // default 60000 => 60s. Set to once per hour
  // End Time Client //

  pinMode(SIGNAL_MONITOR_PIN, INPUT);
  lastPinState = digitalRead(SIGNAL_MONITOR_PIN);

  LedDriver::enteredLoopLed();
}

// the loop function runs over and over again forever
void loop() {
  bool currentPinState = digitalRead(SIGNAL_MONITOR_PIN);
  if(currentPinState != lastPinState)
  {
    uint32_t responseTime = performNtpRequestAndGetResponseTime(timeClient);
  
    if (timeClient.updated())
    {
      //get time stamp
      BYTE timeData[TIME_DATA_BYTE_COUNT];
      getTimeStampAsByteArray(&timeClient, timeData, responseTime);
  
      //send packet with timestamp and slope type
      Packet trigPack(TRIGGER_TYPE, timeData, currentPinState);
      Serial.print(trigPack.buildHexStringPacket());
    }
    else
    {
      //if the time client is not updated send a packet to inform the ntp time client current status
      Packet NTPStatusPack(NTP_STATUS_TYPE, NO_DATA, NTP_STATUS_NOT_UPDATED);
      Serial.print(NTPStatusPack.buildHexStringPacket());
    }
  }
  lastPinState = currentPinState;
    
  //Check physical connection to ethernet
  ethModule.updateLinkStatus(Ethernet.linkStatus());
  if (ethModule.ethStatusChanged())
  {
    Packet checkEth(ETHERNET_CONNECTION_CHECK_TYPE , NO_DATA, ethModule.getStatus());
    Serial.print(checkEth.buildHexStringPacket());
  
    ethModule.handleEthernetConnectionChanges();
  }
  ethModule.setLastStatus(ethModule.getStatus());
}
