#pragma once
#include <cstdint>
#include <cstring>

#define START_BYTE    0xAA
#define RISING_SLOPE  0x01
#define FALLING_SLOPE 0x10
#define EXTRA_BYTES   (uint8_t)3
#define PACKET_OFFSET EXTRA_BYTES - 1
#define FIRST_BYTE    0
#define SECOND_BYTE   1
#define TRIGGER_BYTE  0xAB
#define BOOT_BYTE     0xAC

#define TIME_TO_BYTE_ARRAY_LEN  6                                                //number of time data bytes
#define SLOPE_BYTE              1                                                //number of slope data bytes
#define TYPE_BYTE               1   
#define TYPE_BYTE_POSITION      0                                                //number of packet type data bytes
#define BYTES_BEFORE_TIME_DATA  TYPE_BYTE                                        //number of bytes before time data bytes
#define DATA_BYTE_LENGTH        TYPE_BYTE + TIME_TO_BYTE_ARRAY_LEN + SLOPE_BYTE  //total number of data bytes (type + time + slope)
#define LAST_DATA_BYTE          DATA_BYTE_LENGTH - 1
#define SLOPE_BYTE_POSITION     LAST_DATA_BYTE

class Packet
{
  private:
    uint8_t  startByte;
    uint8_t* dataBytes;
    uint8_t  checkSum;
    uint8_t  dataSize;
    uint8_t* buildPacket;
    char*    builtPacket;
    
  public:
    /**
      * @brief Constructor pentru clasa Packet.
      *
      * Acest constructor inițializează un nou obiect Packet cu datele specificate.
      * Setează byte-ul de start la o valoare predefinită, alocă și copiază datele primite
      * într-un buffer intern, și calculează suma de control pentru pachet.
      *
      * @param data Pointer către array-ul de date care va fi inclus în pachet.
      * @param length Lungimea array-ului de date, în octeți.
      *
      * @note Memoria pentru `dataBytes` este alocată dinamic și este eliberată în deconstructor.
      */
    Packet(const uint8_t* data, uint8_t length);
    
    /**
      * @brief Calculează suma de control pentru pachet.
      * 
      * Inițializează suma de control cu valoarea startByte, apoi aplică XOR între suma de control 
      * curentă și fiecare octet de date din pachet, actualizând suma de control.
      * 
      * Această metodă actualizează membrul 'checkSum' al clasei.
      *
      * @note Această metodă nu are parametri de intrare și nu returnează o valoare, ci
      *       doar actualizează starea internă a obiectului Packet.
      */
    void calculateCheckSum();

    /**
      * @brief Construiește și returnează un pachet de date sub formă de șir de caractere hexazecimale.
      *
      * Funcția generează un pachet de date ce include un octet de start, dimensiunea datelor,
      * datele propriu-zise și o sumă de control, toate acestea fiind apoi convertite în format hexazecimal.
      * Pachetul generat este alocat dinamic și va fi eliberat automat de deconstructorul clasei.
      *
      * @return Un pointer către pachetul hexazecimal construit sub formă de șir de caractere.
      *         Acest șir este terminat cu caracterul null pentru a putea fi utilizat ca și C-string.
      */
    char* buildHexStringPacket();

    /**
      * @brief Deconstructorul pentru clasa Packet.
      *
      * Acest deconstructor eliberează memoria alocată dinamic pentru câmpurile 'dataBytes', 'buildPacket' și 'builtPacket'.
      * Este esențial să te asiguri că obiectul Packet este deconstruit doar după ce pachetul a fost trimis și
      * nu mai este necesar, pentru a evita utilizarea unor referințe invalide la memoria dealocată.
      */
    ~Packet();
};