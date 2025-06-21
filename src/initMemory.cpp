#include "initMemory.h"

// Uncomment to enable serial debug output
// #define MemorydebugSerial

memory_t::memory_t()
  : typeMemory(nullptr)
{}

void memory_t::init(fs::FS *fs) {
  typeMemory = fs;
}

bool memory_t::listDir(const String &dirname, uint8_t levels) {
#ifdef MemorydebugSerial
  Serial.printf("Listing directory: %s\n", dirname.c_str());
#endif
  File root = typeMemory->open(dirname.c_str());
  if (!root || !root.isDirectory()) {
#ifdef MemorydebugSerial
    Serial.println(!root ? "Failed to open directory" : "Not a directory");
#endif
    return false;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
#ifdef MemorydebugSerial
      Serial.print("  DIR : "); Serial.println(file.name());
#endif
      if (levels) listDir(file.name(), levels - 1);
    } else {
#ifdef MemorydebugSerial
      Serial.print("FILE: "); Serial.print(file.name());
      Serial.print(" \tSIZE: "); Serial.println(file.size());
#endif
    }
    file = root.openNextFile();
  }
  return true;
}

bool memory_t::createDir(const String &path) {
#ifdef MemorydebugSerial
  Serial.printf("Creating Dir: %s\n", path.c_str());
#endif
  bool ok = typeMemory->mkdir(path.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "Dir created" : "mkdir failed");
#endif
  return ok;
}

bool memory_t::removeDir(const String &path) {
#ifdef MemorydebugSerial
  Serial.printf("Removing Dir: %s\n", path.c_str());
#endif
  bool ok = typeMemory->rmdir(path.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "Dir removed" : "rmdir failed");
#endif
  return ok;
}

String memory_t::read(const String &path) {
  String dataStr;
#ifdef MemorydebugSerial
  Serial.printf("Reading file: %s\n", path.c_str());
#endif

  File file = typeMemory->open(path.c_str());
  if (!file) {
#ifdef MemorydebugSerial
    Serial.println("Failed to open file for reading");
#endif
    return String("Failed to open file");
  }

  while (file.available()) {
    char c = file.read();
    dataStr += c;
#ifdef MemorydebugSerial
    Serial.write(c);
#endif
  }
  file.close();
  return dataStr;
}

bool memory_t::write(const String &path, const String &message) {
#ifdef MemorydebugSerial
  Serial.printf("Writing file: %s\n", path.c_str());
#endif

  File file = typeMemory->open(path.c_str(), FILE_WRITE);
  if (!file) {
#ifdef MemorydebugSerial
    Serial.println("Failed to open file for writing");
#endif
    return false;
  }
  bool ok = file.print(message.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "File written" : "Write failed");
#endif
  file.close();
  return ok;
}

bool memory_t::append(const String &path, const String &message) {
#ifdef MemorydebugSerial
  Serial.printf("Appending to file: %s\n", path.c_str());
#endif

  File file = typeMemory->open(path.c_str(), FILE_APPEND);
  if (!file) {
#ifdef MemorydebugSerial
    Serial.println("Failed to open file for appending");
#endif
    return false;
  }
  bool ok = file.print(message.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "Message appended" : "Append failed");
#endif
  file.close();
  return ok;
}

bool memory_t::rename(const String &path1, const String &path2) {
#ifdef MemorydebugSerial
  Serial.printf("Renaming file %s to %s\n", path1.c_str(), path2.c_str());
#endif
  bool ok = typeMemory->rename(path1.c_str(), path2.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "File renamed" : "Rename failed");
#endif
  return ok;
}

bool memory_t::remove(const String &path) {
#ifdef MemorydebugSerial
  Serial.printf("Deleting file: %s\n", path.c_str());
#endif
  bool ok = typeMemory->remove(path.c_str());
#ifdef MemorydebugSerial
  Serial.println(ok ? "File deleted" : "Delete failed");
#endif
  return ok;
}
