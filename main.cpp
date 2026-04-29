#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "RFID.h"
#include "TempSensor.h"


const int MAX_ITEMS = 8;
const int MAX_NAME_LEN = 12;

struct Item {
  char name[MAX_NAME_LEN + 1];
  uint8_t location[2];
  int quantity;
  bool used;
};


Item items[MAX_ITEMS];
int itemCount = 0;
bool idleScanning = false;
bool idleScanning2 = false;


String readLine() {
  while (!Serial.available()) delay(10);
  String s = Serial.readStringUntil('\n');
  s.trim();
  return s;
}

int readInt() {
  while (!Serial.available()) delay(10);
  return Serial.parseInt();
}

void inputItem() {
  if (itemCount >= MAX_ITEMS) {
    Serial.println("Inventory full");
    return;
  }
  Serial.println("\nInput New Item Name:");
  String s = readLine();
  s.toCharArray(items[itemCount].name, MAX_NAME_LEN + 1);

  Serial.println("Input Quantity:");
  items[itemCount].quantity = readInt();
  readLine();

  Serial.println("Input Fridge Number:");
  items[itemCount].location[0] = (uint8_t)readInt();
  readLine();

  Serial.println("Input Drawer Number:");
  items[itemCount].location[1] = (uint8_t)readInt();
  readLine();

  items[itemCount].used = true;
  itemCount++;
  Serial.println("\nItem added");
}

void removeItem() {
  Serial.println("\nInput Item to Remove:");
  String s = readLine();
  for (int i = 0; i < itemCount; i++) {
    if (items[i].used && s.equals(String(items[i].name))) {
      for (int j = i; j < itemCount - 1; j++) items[j] = items[j + 1];
      itemCount--;
      Serial.println("\nItem removed");
      return;
    }
  }
  Serial.println("\nItem not found");
}
 
// Print the entire vector
void printItems()
{
  if (itemCount == 0) {
    Serial.println("\nNo items");
    return;
  }
  for (int i = 0; i < itemCount; i++) {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(items[i].name);
    Serial.print(" ");
    Serial.print(items[i].quantity);
    Serial.print(" ");
    Serial.print(items[i].location[0]);
    Serial.print(" ");
    Serial.println(items[i].location[1]);
  }
}

//print location of specific item
void printItemLocation() {
  Serial.println("\nInput Item to Find:");
  String s = readLine();
  for (int i = 0; i < itemCount; i++) {
    if (s.equals(String(items[i].name))) {
      Serial.print("Location: Fridge- ");
      Serial.print(items[i].location[0]); //add fridge and drawer number specification
      Serial.print(", Drawer- ");
      Serial.println(items[i].location[1]);
      return;
    }
  }
  Serial.println("\nItem not found");
}

void clearAllItems() {
  itemCount = 0;
  Serial.println("\nAll items cleared");
}

void RFIDscanningmode() {
  Serial.println("\nEntering RFID scanning mode. Press any key to return to menu.");
  idleScanning = true;
}


/*void TakeTemperature(){
  loopTempSensor();
  delay(500);
}*/


void setup() {
  // Now start serial with extra delay
  Serial.begin(9600);
  
  Serial.println("\nInventory System Ready");
  setupRFID();
  delay(500);
}

void loop() {
  // Poll RFID reader each loop iteration
  loopRFID();
  // If in idle RFID-scanning mode, poll for UIDs and allow exit on key press
  if (idleScanning) {
    String uid;
    if (readCardUID(uid)) {
      Serial.print("Card read: ");
      Serial.println(uid);
    }
    if (Serial.available()) {
      readLine(); // consume input and exit idle mode
      idleScanning = false;
      Serial.println("\nExiting RFID idle mode. Returning to menu.");
    }
    delay(100);
    return;
  }
  

  Serial.println("\n1. Input Item\n2. Remove Item\n3. Print Items\n4. Find Item Location\n5. Clear All Items\n6. Enter RFID Scanning Mode\nEnter choice:");
  delay(100);
  int choice = readInt();
  readLine();

  switch (choice) {
    case 1: inputItem(); break;
    case 2: removeItem(); break;
    case 3: printItems(); break;
    case 4: printItemLocation(); break;
    case 5: clearAllItems(); break;
    case 6: RFIDscanningmode(); break;
    default: Serial.println("Invalid choice");
    
  }
  delay(100);
}