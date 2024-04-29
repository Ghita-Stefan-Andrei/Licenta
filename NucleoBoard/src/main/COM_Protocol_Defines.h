//PACK STRUCTURE CONFIGS
#define START_BYTE              0xAA
#define TYPE_BYTE               1 
#define EXTRA_BYTES             (uint8_t)3  
#define PACKET_OFFSET           EXTRA_BYTES - 1
#define FIRST_BYTE              0
#define SECOND_BYTE             1
#define TYPE_BYTE_POSITION      0                                                //number of packet type data bytes


//BOOT PACK
#define BOOT_TYPE               0xAC

//TRIGGER PACK
#define RISING_SLOPE            0x01
#define FALLING_SLOPE           0x10
#define TRIGGER_TYPE            0xAB
#define TIME_TO_BYTE_ARRAY_LEN  8 //number of time data bytes
#define SLOPE_BYTE              1                                                //number of slope data bytes
#define BYTES_BEFORE_TIME_DATA  TYPE_BYTE                                        //number of bytes before time data bytes
#define DATA_BYTE_LENGTH_TRIG   TYPE_BYTE + TIME_TO_BYTE_ARRAY_LEN + SLOPE_BYTE  //total number of data bytes (type + time + slope)
#define LAST_DATA_BYTE_TRIG     DATA_BYTE_LENGTH_TRIG - 1
#define SLOPE_BYTE_POSITION     LAST_DATA_BYTE


//ETHERNET PACK
#define ETHERNET_STATUS_TYPE    0xAD
#define IP_BYTE_LENGTH          4
#define ETH_STATUS              1
#define BYTES_BEFORE_IP         TYPE_BYTE
#define DATA_BYTE_LENGTH_ETH    TYPE_BYTE + IP_BYTE_LENGTH + ETH_STATUS
#define LAST_DATA_BYTE_ETH      DATA_BYTE_LENGTH_ETH - 1
#define ETH_CONNECTED           0x01
#define ETH_NOT_CONNECTED       0x00