#pragma once

#include <Arduino.h>
#include <FS.h>

// Uncomment to enable serial debug
// #define Debug_Serial_Memory

/**
 * @brief A lightweight FS helper class for ESP32 (SPIFFS, LittleFS, SD).
 * @version 2.0.0
 * @author Milad Nikpendar
 */
class memoryAccess_t {
public:
  /**
   * @brief Construct a new memoryAccess_t object and bind FS.
   * @param initMemory pointer to SPIFFS, LittleFS or SD instance
   */
  memoryAccess_t(fs::FS *initMemory);

  bool type(uint8_t cardType);
  bool listDir(const char *dirname, uint8_t levels);
  bool createDir(const char *path);
  bool removeDir(const char *path);
  String read(const char *path);
  bool write(const char *path, const char *message);
  bool append(const char *path, const char *message);
  bool rename(const char *path1, const char *path2);
  bool remove(const char *path);
  void testIO(const char *path);

private:
  fs::FS *typeMemory;
};
