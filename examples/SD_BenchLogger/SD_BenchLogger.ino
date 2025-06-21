/******************************************************************************
  Example 02: SD Bench & Data Logger v2.0.0
  Board:    ESP32
  FS:       SD
  Library:  initMemory v2.0.0
  Shows:
    • SD.begin()
    • memoryAccess_t constructor & type detection
    • Creating a folder, appending log entries
    • Reading back the log file
    • Running testIO() benchmark
******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "initMemory.h"

// enable verbose serial debug
#define Debug_Serial_Memory

// adjust to your board’s CS pin
static const int SD_CS = 5;

// bind our class to the SD interface
memoryAccess_t storage(&SD);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("\n--- SD Bench & Data Logger ---");

  // 1) init SD
  if (!SD.begin(SD_CS)) {
    Serial.println("❌ SD card init failed");
    return;
  }
  Serial.println("✅ SD card mounted");

  // 2) detect SD type
  uint8_t cardType = SD.cardType();
  storage.type(cardType);

  // 3) prepare data folder
  storage.removeDir("/datalog");      // clean up if rerunning
  storage.createDir("/datalog");

  // 4) log dummy entries
  Serial.println("\n» Writing log entries...");
  for (int i = 0; i < 10; i++) {
    char buf[64];
    snprintf(buf, sizeof(buf),
             "Entry #%02d @ ms %lu", i + 1, millis());
    storage.append("/datalog/log.txt", buf);
    storage.append("/datalog/log.txt", "\n");
    delay(300);
  }
  Serial.println("✅ Logged 10 entries");

  // 5) read back the log
  Serial.println("\n» Reading back /datalog/log.txt:");
  String logContent = storage.read("/datalog/log.txt");
  Serial.println(logContent);

  // 6) run I/O benchmark
  Serial.println("\n» Running I/O benchmark...");
  storage.testIO("/datalog/benchmark.bin");

  Serial.println("\n✅ Example complete.");
}

void loop() {
  // all work done in setup()
}
