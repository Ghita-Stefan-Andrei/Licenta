#include "main.h"

EthernetUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool lastPinState = LOW;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  // Ethernet Set Up //
  /*Serial.print(F("\nStart Ethernet_NTPClient_Basic_STM32 on ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.println(SHIELD_TYPE);
  Serial.println(ETHERNET_WEBSERVER_STM32_VERSION);
  Serial.println(NTPCLIENT_GENERIC_VERSION);*/

  initEthernet();
  // End Ethernet //

  // Time Client for NTP request Set UP //
  timeClient.begin();
  timeClient.setTimeOffset(3600 * TIME_ZONE_OFFSET_HRS);
  // default 60000 => 60s. Set to once per hour
  timeClient.setUpdateInterval(SECS_IN_HR);
  
  //Serial.println("Using NTP Server " + timeClient.getPoolServerName());
  // End Time Client //

  pinMode(PC8, INPUT);
  lastPinState = digitalRead(PC8);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, HIGH);

  uint8_t data[TYPE_BYTE]; data[0] = BOOT_BYTE;
  Packet pack(data, sizeof(data));
  Serial.print(pack.buildHexStringPacket());
}

// the loop function runs over and over again forever
void loop() {
  bool currentPinState = digitalRead(PC8);
  if(currentPinState != lastPinState)
  {
    timeClient.update();
    if (timeClient.updated())
    {
      uint8_t data[TIME_TO_BYTE_ARRAY_LEN + SLOPE_BYTE + TYPE_BYTE];
      data[0] = TRIGGER_BYTE;
      getTimeStampAsByteArray(&timeClient, data);

      if(currentPinState == HIGH) data[LAST_DATA_BYTE] = RISING_SLOPE;
      if(currentPinState == LOW)  data[LAST_DATA_BYTE] = FALLING_SLOPE;

      Packet pack(data, sizeof(data));
      Serial.print(pack.buildHexStringPacket());
    }
  }
  lastPinState = currentPinState;
}
