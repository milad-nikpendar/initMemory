/******************************************************************************
  Example: FS_Explorer v3.0.0 (LittleFS)
  Board:    ESP32 DevKitC
  FS:       LittleFS
  Library:  initMemory & initTransport v3.0.0
  Demo:     directory listing, file I/O & benchmark
******************************************************************************/

#include <Arduino.h>
#include <LittleFS.h>
#define Debug_Serial_Memory
#include <initMemory.h>

// bind LittleFS to our core FS helper
memoryAccess_t Filemanager(&LittleFS);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("\n--- FS_Explorer (LittleFS) Demo ---");

  // 1) Mount LittleFS (auto-format if empty)
  if (!LittleFS.begin(true)) {
    Serial.println("❌ Failed to mount LittleFS");
    return;
  }
  Serial.println("✅ LittleFS mounted");

  // 2) Prepare demo folder
  const char* demoDir = "/fs_explorer";
  Filemanager.removeDir(demoDir);        // clean up previous runs
  Filemanager.createDir(demoDir);

  // 3) Show directory tree (depth=2)
  Serial.println("\nDirectory tree:");
  Filemanager.listDir("/", 2);

  // 4) Write a demo file
  const char* demoFile = "/fs_explorer/info.txt";
  Filemanager.write(demoFile,
           "FS Explorer v3 Demo (LittleFS)\r\n"
           "Line 1: Hello\r\n"
           "Line 2: World\r\n");
  Serial.printf("\nWrote file: %s\n", demoFile);

  // 5) Check existence & get size via class method
  bool exists = Filemanager.exists(demoFile);
  Serial.printf("Exists: %s → %s\n", demoFile, exists ? "yes" : "no");
  if (exists) {
    File f = Filemanager.open(demoFile);
    size_t len = Filemanager.size(f);
    f.close();
    Serial.printf("Size: %u bytes\n", len);
  }

  // 6) Read entire file
  Serial.println("\nFull contents:");
  Serial.println(Filemanager.read(demoFile));

  // 7) Read first 2 lines only
  Serial.println("First two lines:");
  File fl = Filemanager.open(demoFile);
  bool end_file;
  Serial.println(Filemanager.readLines(fl, 2, end_file));
  fl.close();

  // 8) Run I/O benchmark
  Serial.println("I/O benchmark:");
  Filemanager.testIO("/fs_explorer/bench.bin");

  Serial.println("\n✅ FS_Explorer (LittleFS) complete");
}

void loop() {
  // nothing here
  delay(1000);
}
