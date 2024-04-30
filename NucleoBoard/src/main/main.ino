#include "main.h"

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool lastPinState = LOW;
ethInfo ethInfoStatus;

void setup() {
  pinMode(LED_RED, OUTPUT);    //while the RED led is on the board is in setup, once it closes and the BLUE led lights up the board enters the loop
  digitalWrite(LED_RED, HIGH);

  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Packet bootPack(BOOT_TYPE);
  Serial.print(bootPack.buildHexStringPacket());

  ethInfoStatus = initEthernet();

  uint8_t ip[IP_BYTE_LENGTH];
  decodeIP(ip, ethInfoStatus.ip);

  Packet ethPack(ETHERNET_STATUS_TYPE, ip, ethInfoStatus.status);
  Serial.print(ethPack.buildHexStringPacket());

  // End Ethernet //

  // Time Client for NTP request Set UP //
  timeClient.begin();
  timeClient.setTimeOffset(3600 * TIME_ZONE_OFFSET_HRS);
  // default 60000 => 60s. Set to once per hour
  timeClient.setUpdateInterval(SECS_IN_HR);
  
  // End Time Client //

  pinMode(SIGNAL_MONITOR_PIN, INPUT);
  lastPinState = digitalRead(SIGNAL_MONITOR_PIN);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_RED, LOW);
}

EthernetLinkStatus lastEthStatus = !(ethInfoStatus.status) ? LinkON : LinkOFF;

// the loop function runs over and over again forever
void loop() {
  bool currentPinState = digitalRead(SIGNAL_MONITOR_PIN);
  if(currentPinState != lastPinState)
  {
    uint32_t serverResponseDelay1 = millis();
    timeClient.update();
    uint32_t serverResponseDelay2 = millis();

    uint32_t serverResponseTime = serverResponseDelay2 - serverResponseDelay1;

    if (timeClient.updated())
    {
      uint8_t slopeType = 0x00;
      uint8_t timeData[TIME_TO_BYTE_ARRAY_LEN];

      getTimeStampAsByteArray(&timeClient, timeData, serverResponseTime);

      if(currentPinState == HIGH) slopeType = RISING_SLOPE;
      if(currentPinState == LOW)  slopeType = FALLING_SLOPE;

      Packet trigPack(TRIGGER_TYPE, timeData, slopeType);
      Serial.print(trigPack.buildHexStringPacket());
    }
  }
  lastPinState = currentPinState;
  
  EthernetLinkStatus status = Ethernet.linkStatus();
  if (status != lastEthStatus)
  {
    Packet checkEth(ETHERNET_STATUS_CHECK_T, NO_DATA, status);
    Serial.print(checkEth.buildHexStringPacket());
  }
  lastEthStatus = status;
}
