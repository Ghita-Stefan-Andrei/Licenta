#pragma once
#include <cstdint>
#include <cstring>
#include "COM_Protocol_Defines.h"

class Packet
{
  private:
    uint8_t  startByte;
    uint8_t* dataBytes;
    uint8_t  checkSum;
    uint8_t  dataSize;
    uint8_t* buildPacket;
    char*    builtPacket;

  private:
    /**
      * @brief Creează un pachet de trigger cu datele de timp și informații despre panta.
      * 
      * Această funcție inițializează array-ul de date pentru pachet, stabilește dimensiunea acestuia,
      * plasează un tip predefinit de pachet de trigger în poziția de byte type specificată,
      * copiază datele de timp furnizate începând de la o poziție de byte specifică după byte type,
      * și setează byte-ul pantei ca ultim byte de date al pachetului.
      *
      * @param timeData Pointer către array-ul care conține datele de timp.
      * @param slopeByte Byte-ul care reprezintă informația despre pantă.
      */
    void createTriggerPacket(uint8_t* dataByteArr, uint8_t extraByte);

    /**
      * @brief Creează un pachet de boot.
      * 
      * Această funcție alocă memoria pentru pachetul de boot, stabilește dimensiunea acestuia,
      * și atribuie tipul de pachet de boot la poziția byte-ului de tip.
      */
    void createBootPacket();

    /**
      * @brief Creează un pachet de stare Ethernet cu adresa IP și starea conexiunii.
      * 
      * Această funcție inițializează array-ul de date pentru pachetul Ethernet, stabilește dimensiunea acestuia,
      * atribuie tipul de pachet de stare Ethernet în poziția predefinită, copiază adresa IP furnizată
      * în pachet și setează starea conexiunii Ethernet ca ultim byte al pachetului.
      *
      * @param ipAdress Pointer către array-ul care conține adresa IP.
      * @param ethStatus Starea conexiunii Ethernet (conectat sau neconectat).
      */
    void createEthernetPacket(uint8_t* dataByteArr, uint8_t extraByte, uint8_t type);

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
      * @brief Constructor pentru crearea tipurilor specifice de pachete bazate pe parametrii de intrare.
      * 
      * Acest constructor setează byte-ul de început și inițiază crearea pachetului pe baza
      * tipului de pachet furnizat (de exemplu, trigger, boot sau Ethernet). De asemenea, calculează checksum-ul
      * pentru pachetul creat.
      *
      * @param packetType Tipul de pachet care urmează să fie creat.
      * @param dataByteArr Pointer către un array de bytes de date utilizat pentru crearea pachetului (variază în funcție de tipul pachetului).
      * @param extraByte Un byte extra utilizat în unele tipuri de pachete pentru a transmite informații suplimentare (precum tipul pantei pt pachete trigger, sau statusul conexiunii ethernet pt pachete ethernet).
      */
    Packet(const uint8_t packetType, uint8_t* dataByteArr = nullptr, uint8_t extraByte = 0x00);

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