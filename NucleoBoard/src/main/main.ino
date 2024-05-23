#include "main.h"

void setup() {
  pinMode(LED_RED,  OUTPUT);    //the RED led marks the connection to ethernet. If it is on, the board is not connected to ethernet.
  pinMode(LED_BLUE, OUTPUT);    //the BLUE led marks when the board enters the loop function

  Serial.begin(BIT_RATE);
  while (!Serial && millis() < SERIAL_OPEN_TIMEOUT);
  
  Packet bootPack(BOOT_TYPE);
  Serial.print(bootPack.buildHexStringPacket());

  // Ethernet
  ethInfoStatus = initEthernet();

  uint8_t ip[IP_BYTE_LENGTH];
  decodeIP(ip, ethInfoStatus.ip);

  Packet ethPack(ETHERNET_STATUS_TYPE, ip, ethInfoStatus.status);
  Serial.print(ethPack.buildHexStringPacket());

  lastEthStatus = Ethernet.linkStatus();

  switch(lastEthStatus)
  {
    case LinkON : digitalWrite(LED_RED, LOW); break;
    case LinkOFF: digitalWrite(LED_RED, HIGH); break;
    default: break;
  }

  // End Ethernet //
  timeClient.begin();
  timeClient.setTimeOffset(SECONDS_IN_HOUR * TIME_ZONE_OFFSET_HRS);
  timeClient.setUpdateInterval(SECS_IN_HR); // default 60000 => 60s. Set to once per hour
  // End Time Client //

  pinMode(SIGNAL_MONITOR_PIN, INPUT);
  lastPinState = digitalRead(SIGNAL_MONITOR_PIN);

  digitalWrite(LED_BLUE, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  bool currentPinState = digitalRead(SIGNAL_MONITOR_PIN);
  if(currentPinState != lastPinState)
  {
    uint32_t updatedMillis = performNtpRequestAndCalibrateMlisecs(timeClient);

    if (timeClient.updated())
    {
      //get time stamp
      BYTE timeData[TIME_DATA_BYTE_COUNT];
      getTimeStampAsByteArray(&timeClient, timeData, updatedMillis);

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
  EthernetLinkStatus status = Ethernet.linkStatus();
  if (status != lastEthStatus)
  {
    Packet checkEth(ETHERNET_CONNECTION_CHECK_TYPE , NO_DATA, status);
    Serial.print(checkEth.buildHexStringPacket());

    switch(status)
    {
      case LinkOFF: digitalWrite(LED_RED, HIGH); break;  //Light up the red led when the connection is broken
      case LinkON : NVIC_SystemReset(); break;           //Reset the board to try to reconnect to internet after an ethernet connection is reestablished
      default: break;
    }
  }
  lastEthStatus = status;
}
