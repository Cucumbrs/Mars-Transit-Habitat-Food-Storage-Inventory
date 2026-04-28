#include "RFID.h"
#include <SPI.h>

#define RST_PIN 6
#define SS_PIN  18

MFRC522 mfrc522(SS_PIN, RST_PIN);
bool rfidConnected = false;

void setupRFID() {
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  
  // Check if RFID module is connected
  /*byte version = mfrc522.PCD_ReadRegister(MFRC522::PCD_Register::VersionReg);
  if (version == 0 || version == 0xFF) {
    Serial.println("ERROR: RFID module not detected!");
    rfidConnected = false;
    return;
  }
  
  rfidConnected = true;*/
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loopRFID() {
  /*if (!rfidConnected) return;  // Skip if not connected*/
  
  delay (100);
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

bool readCardUID(String &uid) {
  //if (!rfidConnected) return false;  // Skip if not connected
  
  if (!mfrc522.PICC_IsNewCardPresent()) return false;
  if (!mfrc522.PICC_ReadCardSerial()) return false;
  uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(mfrc522.uid.uidByte[i], HEX);
    if (i + 1 < mfrc522.uid.size) uid += ":";
  }
  // Normalize to upper-case hex
  for (unsigned int i = 0; i < uid.length(); ++i) {
    char c = uid.charAt(i);
    if (c >= 'a' && c <= 'f') uid.setCharAt(i, c - 'a' + 'A');
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  return true;
  delay (100);
}
