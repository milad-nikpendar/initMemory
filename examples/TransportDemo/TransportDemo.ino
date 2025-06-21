/******************************************************************************
  Example: TransportDemo v3.0.0
  Board:    ESP32 DevKitC
  FS:       LittleFS → SD
  Library:  initMemory & initTransport v3.0.0
  Demo:     create file on LittleFS, then copy to SD with one transportAccess_t
******************************************************************************/

#include <Arduino.h>
#include <LittleFS.h>
#include <SPI.h>
#include <SD.h>
#define Debug_Serial_Memory
#include "initMemory.h"
#include "initTransport.h"

// SD card chip‐select pin
static const int SD_CS = 5;

// bind LittleFS to our transport+FS helper
transportAccess_t transporter(&LittleFS);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("\n--- TransportDemo (LittleFS → SD) ---");

  // 1) Mount LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("❌ LittleFS mount failed");
    return;
  }
  Serial.println("✅ LittleFS mounted");

  // 2) Mount SD
  if (!SD.begin(SD_CS)) {
    Serial.println("❌ SD init failed");
    return;
  }
  Serial.println("✅ SD mounted");

  // 3) Create a test file on LittleFS
  const char* filename = "demo.txt";   // cargo name
  transporter.write("/demo.txt",
    "TransportDemo v3\n"
    "  Created on LittleFS\n"
    "  Will copy to SD\n"
  );
  Serial.println("→ File created in LittleFS: /demo.txt");

  // 4) Create destination folder on SD
  if (!SD.exists("/copied")) {
    SD.mkdir("/copied");
    Serial.println("→ Created SD folder: /copied");
  }

  // 5) Configure origin (LittleFS, root dir + filename) and goal (SD, target folder)
  transporter.origin_setup(&LittleFS, "/", filename, false);
  transporter.goal_setup(&SD, "/copied");

  // 6) Copy file (no cut)
  if (transporter.copyFile()) {
    Serial.println("✅ demo.txt copied to SD:/copied/");
  } else {
    Serial.println("❌ Copy failed");
    return;
  }

  // 7) Verify on SD
  const char* target = "/copied/demo.txt";
  if (SD.exists(target)) {
    Serial.printf("✅ Verified: %s exists on SD\n", target);
    File f = SD.open(target);
    size_t n = transporter.size(f);
    f.close();
    Serial.printf("Size on SD: %u bytes\n", n);
  }

  Serial.println("\n--- TransportDemo complete ---");
}

void loop() {
  // nothing here
}
