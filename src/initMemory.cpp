// initMemory.cpp
#include "initMemory.h"

// #define Debug_Serial_Memory

memoryAccess_t::memoryAccess_t(fs::FS *initMemory)
  : typeMemory(initMemory)
{}

bool memoryAccess_t::type(uint8_t cardType) {
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return false;
  }

  Serial.print("SD Card Type: ");
  switch (cardType) {
    case CARD_MMC:  Serial.println("MMC");  break;
    case CARD_SD:   Serial.println("SDSC"); break;
    case CARD_SDHC: Serial.println("SDHC"); break;
    default:        Serial.println("UNKNOWN"); break;
  }
  return true;
}

bool memoryAccess_t::listDir(const char *dirname, uint8_t levels) {
#ifdef Debug_Serial_Memory
  Serial.printf("Listing directory: %s\n", dirname);
#endif
  File root = typeMemory->open(dirname);
  if (!root || !root.isDirectory()) {
#ifdef Debug_Serial_Memory
    Serial.println(!root ? "Failed to open directory" : "Not a directory");
#endif
    return false;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
#ifdef Debug_Serial_Memory
      Serial.print("  DIR : "); Serial.println(file.name());
#endif
      if (levels) listDir(file.name(), levels - 1);
    } else {
#ifdef Debug_Serial_Memory
      Serial.print("FILE: "); Serial.print(file.name());
      Serial.print(" \tSIZE: "); Serial.println(file.size());
#endif
    }
    file = root.openNextFile();
  }
  return true;
}

bool memoryAccess_t::createDir(const char *path) {
#ifdef Debug_Serial_Memory
  Serial.printf("Creating Dir: %s\n", path);
#endif
  bool ok = typeMemory->mkdir(path);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "Dir created" : "mkdir failed");
#endif
  return ok;
}

bool memoryAccess_t::removeDir(const char *path) {
#ifdef Debug_Serial_Memory
  Serial.printf("Removing Dir: %s\n", path);
#endif
  bool ok = typeMemory->rmdir(path);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "Dir removed" : "rmdir failed");
#endif
  return ok;
}

String memoryAccess_t::read(const char *path) {
#ifdef Debug_Serial_Memory
  Serial.printf("Reading file: %s\n", path);
#endif
  File file = typeMemory->open(path);
  if (!file) {
#ifdef Debug_Serial_Memory
    Serial.println("Failed to open file for reading");
#endif
    return "Failed to open file";
  }

  String data;
  while (file.available()) {
    char c = file.read();
    data += c;
#ifdef Debug_Serial_Memory
    Serial.write(c);
#endif
  }
  file.close();
  return data;
}

bool memoryAccess_t::write(const char *path, const char *message) {
#ifdef Debug_Serial_Memory
  Serial.printf("Writing file: %s\n", path);
#endif
  File file = typeMemory->open(path, FILE_WRITE);
  if (!file) {
#ifdef Debug_Serial_Memory
    Serial.println("Failed to open file for writing");
#endif
    return false;
  }
  bool ok = file.print(message);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "File written" : "Write failed");
#endif
  file.close();
  return ok;
}

bool memoryAccess_t::append(const char *path, const char *message) {
#ifdef Debug_Serial_Memory
  Serial.printf("Appending to file: %s\n", path);
#endif
  File file = typeMemory->open(path, FILE_APPEND);
  if (!file) {
#ifdef Debug_Serial_Memory
    Serial.println("Failed to open file for appending");
#endif
    return false;
  }
  bool ok = file.print(message);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "Message appended" : "Append failed");
#endif
  file.close();
  return ok;
}

bool memoryAccess_t::rename(const char *path1, const char *path2) {
#ifdef Debug_Serial_Memory
  Serial.printf("Renaming file %s to %s\n", path1, path2);
#endif
  bool ok = typeMemory->rename(path1, path2);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "File renamed" : "Rename failed");
#endif
  return ok;
}

bool memoryAccess_t::remove(const char *path) {
#ifdef Debug_Serial_Memory
  Serial.printf("Deleting file: %s\n", path);
#endif
  bool ok = typeMemory->remove(path);
#ifdef Debug_Serial_Memory
  Serial.println(ok ? "File deleted" : "Delete failed");
#endif
  return ok;
}

void memoryAccess_t::testIO(const char *path) {
  static uint8_t buf[512];
  Serial.print("- writing");
  uint32_t start = millis();
  File file = typeMemory->open(path, FILE_WRITE);
  for (size_t i = 0; i < 2048; i++) {
    if ((i & 0x1F) == 0x1F) Serial.print(".");
    file.write(buf, 512);
  }
  file.close();
  uint32_t writeTime = millis() - start;
  Serial.printf("\n- %u bytes written in %u ms\n", 2048UL*512, writeTime);
  Serial.printf("  write speed: %u B/ms\n", (2048UL*512)/writeTime);

  Serial.print("- reading");
  start = millis();
  file = typeMemory->open(path);
  if (file && !file.isDirectory()) {
    size_t remaining = file.size(), total = remaining;
    while (remaining) {
      size_t toRead = remaining > 512 ? 512 : remaining;
      file.read(buf, toRead);
      if ((total-remaining & 0x1F) == 0x1F) Serial.print(".");
      remaining -= toRead;
    }
    file.close();
    uint32_t readTime = millis() - start;
    Serial.printf("\n- %u bytes read in %u ms\n", total, readTime);
    Serial.printf("  read speed: %u B/ms\n", total/readTime);
  } else {
    Serial.println("\n- failed to open file for reading");
  }
}
