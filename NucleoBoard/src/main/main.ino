#include "main.h"

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool lastPinState = LOW;
bool reconected = false;
ethInfo ethInfoStatus;
EthernetLinkStatus lastEthStatus;

void setup() {
  pinMode(LED_RED,  OUTPUT);    //the RED led marks the connection to ethernet. If it is on, the board is not connected to ethernet.
  pinMode(LED_BLUE, OUTPUT);    //the BLUE led marks when the board enters the loop function

  Serial.begin(115200);
  while (!Serial && millis() < 5000);
  
  Packet bootPack(BOOT_TYPE);
  Serial.print(bootPack.buildHexStringPacket());

  // Ethernet
  ethInfoStatus = initEthernet();

  uint8_t ip[IP_BYTE_LENGTH];
  decodeIP(ip, ethInfoStatus.ip);

  Packet ethPack(ETHERNET_STATUS_TYPE, ip, ethInfoStatus.status);
  Serial.print(ethPack.buildHexStringPacket());

  lastEthStatus = Ethernet.linkStatus();

  if (lastEthStatus == LinkON)  digitalWrite(LED_RED, LOW);
  if (lastEthStatus == LinkOFF) digitalWrite(LED_RED, HIGH);

  // End Ethernet //
  timeClient.begin();
  timeClient.setTimeOffset(3600 * TIME_ZONE_OFFSET_HRS);
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
      BYTE timeData[TIME_TO_BYTE_ARRAY_LEN];
      getTimeStampAsByteArray(&timeClient, timeData, updatedMillis);

      //send packet with timestamp and slope type
      Packet trigPack(TRIGGER_TYPE, timeData, currentPinState);
      Serial.print(trigPack.buildHexStringPacket());
    }
    else
    {
      Packet NTPStatusPack(NTP_STATUS_TYPE, NO_DATA, NTP_STATUS_NOT_UPDATED);
      Serial.print(NTPStatusPack.buildHexStringPacket());
    }
  }
  lastPinState = currentPinState;
  
  //Check physical connection to ethernet
  EthernetLinkStatus status = Ethernet.linkStatus();
  if (status != lastEthStatus)
  {
    Packet checkEth(ETHERNET_STATUS_CHECK_T, NO_DATA, status);
    Serial.print(checkEth.buildHexStringPacket());

    if (status == LinkOFF) digitalWrite(LED_RED, HIGH);  //Light up the red led when the connection is broken
    if (status == LinkON)  NVIC_SystemReset();           //Reset the board to try to reconnect to internet after an ethernet connection is reestablished
  }
  lastEthStatus = status;
}
