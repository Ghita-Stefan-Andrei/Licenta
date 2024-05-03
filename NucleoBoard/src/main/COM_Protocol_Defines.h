//PACK STRUCTURE CONFIGS
#define START_BYTE              0xAA                                             //The byte that represent the start of the pack, every pack should start with this exact byte
#define TYPE_BYTE               1                                                //How many bytes to mark the type of a pack (used to calculate data lengths and last positions)
#define EXTRA_BYTES             (uint8_t)3                                       //Number of bytes that are not data bytes (START_BYTE, LENGTH_BYTE, CHECK_SUM_BYTE)
#define PACKET_OFFSET           EXTRA_BYTES - 1                                  //The bytes before the data bytes in a pack
#define FIRST_BYTE              0                                                //The first position in the packet, used to write the START_BYTE 0xAA
#define SECOND_BYTE             1                                                //The second position in the packet, used to write the length of the data in the pack
#define TYPE_BYTE_POSITION      0                                                //Position of the packet type byte in the data array
#define NO_DATA                 {}                                               //Used for clarity if sending a packet with an extraDataByte and no aditional data

//TRIGGER PACK
#define TRIGGER_TYPE            0xAB                                             //Type of packet that tells when the signal changed state
#define RISING_SLOPE            0x01                                             //Byte marking that the signal went from 0 to 1
#define FALLING_SLOPE           0x10                                             //Byte marking that the signal went from 1 to 0
#define TIME_TO_BYTE_ARRAY_LEN  8                                                //number of time data bytes
#define SLOPE_BYTE              1                                                //number of slope data bytes
#define BYTES_BEFORE_TIME_DATA  TYPE_BYTE                                        //number of bytes before time data bytes
#define DATA_BYTE_LENGTH_TRIG   TYPE_BYTE + TIME_TO_BYTE_ARRAY_LEN + SLOPE_BYTE  //total number of data bytes (type + time + slope)
#define LAST_DATA_BYTE_TRIG     DATA_BYTE_LENGTH_TRIG - 1                        //Position of the last byte in trigg data array
#define SLOPE_BYTE_POSITION     LAST_DATA_BYTE_TRIG                              //Position of the byte that indicates the slope type

//BOOT PACK
#define BOOT_TYPE               0xAC                                             //Type of packet that is sent when the board boots up

//ETHERNET PACK
#define ETHERNET_STATUS_TYPE    0xAD                                             //Type of packet that tells if the board had connected to internet during set up                             
#define ETHERNET_STATUS_CHECK_T 0xAE                                             //Type of packet that tells if the board is still connected to ethernet during program execution
#define IP_BYTE_LENGTH          4                                                //Number of bytes of the IPv4 of the board
#define ETH_STATUS              1                                                //Number of bytes the ethernet status takes
#define BYTES_BEFORE_IP         TYPE_BYTE                                        //Number of bytes before the IP bytes in the data segment
#define DATA_BYTE_LENGTH_ETH    TYPE_BYTE + IP_BYTE_LENGTH + ETH_STATUS          //Number of bytes of the data segment of the packet ETHERNET_STATUS_TYPE
#define LAST_DATA_BYTE_ETH      DATA_BYTE_LENGTH_ETH - 1                         //Position of the last data byte in the array of data bytes of ETHERNET_STATUS_TYPE
#define ETH_STATUS_BYTE_POS     LAST_DATA_BYTE_ETH                               //Position of the status of ethernet in ETHERNET_STATUS_TYPE
#define ETH_CONNECTED           0x01                                             //Connected to eth
#define ETH_NOT_CONNECTED       0x00                                             //Not connected to eth
#define DATA_BYTE_LENGTH_ETH_C  TYPE_BYTE + ETH_STATUS                           //Number of bytes of the data segment of the packet ETHERNET_STATUS_CHECK_T
#define ETH_STATUS_BYTE_POS_C   DATA_BYTE_LENGTH_ETH_C - 1                       //Position of the last data byte in the array of data bytes of ETHERNET_STATUS_CHECK_T
#define ETH_CONNECTION_OFF      0x02                                             //Connected to ethernet (value 0x02 to match enum LinkON from STM32Ethernet.h)
#define ETH_CONNECTION_UNKNOWN  0x03                                             //Connected to ethernet (value 0x03 to match enum LinkON from STM32Ethernet.h)

//NTP server availability
#define NTP_STATUS_TYPE         0xAF                                             //Type of packet that tells if the NTP CLient on the board has been updated or not
#define NTP_STATUS_UPDATED      0xFF                                             //Status of the NTP Client, 0xFF means it has been updated
#define NTP_STATUS_NOT_UPDATED  0x00                                             //Status of the NTP Client, 0x00 means it has not been updated
#define NTP_STATUS              1                                                //Number of bytes used in the packet for the NTP Client status
#define DATA_BYTE_LENGTH_NTP_S  TYPE_BYTE + NTP_STATUS                           //Number of bytes of the data payload
#define LAST_BYTE_POSITION      DATA_BYTE_LENGTH_NTP_S - 1                       //The position of the last data byte
#define NTP_STATUS_POSITION     LAST_BYTE_POSITION                               //The position of the NTP CLient status byte
typedef uint8_t BYTE;