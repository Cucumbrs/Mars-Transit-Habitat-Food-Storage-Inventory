#ifndef RFID_H
#define RFID_H

#include <MFRC522.h>

// Initialize RFID hardware (call once in setup)
void setupRFID();

// Poll RFID reader (call frequently in loop)
void loopRFID();

// Non-blocking read: returns true and fills `uid` when a new card is read
bool readCardUID(String &uid);

#endif // RFID_H
