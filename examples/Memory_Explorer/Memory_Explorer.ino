/******************************************************************************
  Example: initMemory v2.0.0
  Board:    ESP32
  FS:       SPIFFS
  Library:  initMemory v2.0.0
******************************************************************************/

#include <Arduino.h>
#include <SPIFFS.h>

// enable verbose serial debug
#define Debug_Serial_Memory
#include "initMemory.h"

// bind our class to the SPIFFS interface
memoryAccess_t storage(&SPIFFS);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("\n--- initMemory v2.0.0 Example ---");

  // 1. Mount SPIFFS (auto-format if empty)
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ Failed to mount SPIFFS!");
    while (true) delay(1000);
  }
  Serial.println("✅ SPIFFS mounted.");

  // 2. Directory operations
  Serial.println("\n» Creating /testDir");
  storage.createDir("/testDir");

  Serial.println("\n» Listing / (root), depth=0");
  storage.listDir("/", 0);

  // 3. File write & read
  Serial.println("\n» Writing /testDir/hello.txt");
  storage.write("/testDir/hello.txt", "Hello initMemory v2!");

  Serial.println("\n» Reading /testDir/hello.txt");
  String content = storage.read("/testDir/hello.txt");
  Serial.println(">> " + content);

  // 4. Append
  Serial.println("\n» Appending to /testDir/hello.txt");
  storage.append("/testDir/hello.txt", "\n— appended line");

  Serial.println("\n» Reading again");
  content = storage.read("/testDir/hello.txt");
  Serial.println(">> " + content);

  // 5. Rename
  Serial.println("\n» Renaming to /testDir/greeting.txt");
  storage.rename("/testDir/hello.txt", "/testDir/greeting.txt");
  storage.listDir("/testDir", 0);

  // 6. Delete file
  Serial.println("\n» Deleting /testDir/greeting.txt");
  storage.remove("/testDir/greeting.txt");
  storage.listDir("/testDir", 0);

  // 7. Remove directory
  Serial.println("\n» Removing /testDir");
  storage.removeDir("/testDir");
  storage.listDir("/", 0);

  Serial.println("\n✅ All operations complete.");
}

void loop() {
  // nothing to do here
}
