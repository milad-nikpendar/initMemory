/******************************************************************************
  Example: initMemory v1.0.0
  Board:    ESP32
  FS:       SPIFFS
  Library:  initMemory (https://github.com/milad-nikpendar/initMemory)
******************************************************************************/
#include <Arduino.h>
#include <SPIFFS.h>
#include "initMemory.h"

// Uncomment to enable verbose serial debug
// #define MemorydebugSerial

memory_t memory;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- initMemory v1.0.0 Example ---");

  // 1. Mount SPIFFS
  if(!SPIFFS.begin(true)) {
    Serial.println("❌ Failed to mount SPIFFS!");
    while(true) delay(1000);
  }
  Serial.println("✅ SPIFFS mounted.");

  // 2. Initialize library
  memory.init(&SPIFFS);

  // 3. Directory ops
  Serial.println("\n» Create directory /testDir");
  memory.createDir("/testDir");

  Serial.println("\n» List root directory:");
  memory.listDir("/", 0);

  // 4. File write & read
  Serial.println("\n» Write to /testDir/hello.txt");
  memory.write("/testDir/hello.txt", "Hello initMemory!");

  Serial.println("\n» Read from /testDir/hello.txt");
  String content = memory.read("/testDir/hello.txt");
  Serial.println(">> " + content);

  // 5. Append
  Serial.println("\n» Append to /testDir/hello.txt");
  memory.append("/testDir/hello.txt", "\nAppended line.");

  Serial.println("\n» Read again:");
  content = memory.read("/testDir/hello.txt");
  Serial.println(">> " + content);

  // 6. Rename file
  Serial.println("\n» Rename file to /testDir/greeting.txt");
  memory.rename("/testDir/hello.txt", "/testDir/greeting.txt");
  memory.listDir("/testDir", 0);

  // 7. Delete file
  Serial.println("\n» Remove file /testDir/greeting.txt");
  memory.remove("/testDir/greeting.txt");
  memory.listDir("/testDir", 0);

  // 8. Remove directory
  Serial.println("\n» Remove directory /testDir");
  memory.removeDir("/testDir");
  memory.listDir("/", 0);

  Serial.println("\n✅ All operations done.");
}

void loop() {
  // nothing to do here
}
