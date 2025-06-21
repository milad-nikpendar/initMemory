# initMemory v2.0.0

A lightweight and feature-rich file system utility for ESP32-based projects.  
Built on top of the Arduino FS interface, `initMemory` v2 introduces SD-card type detection, I/O benchmarking, and a cleaner C-style API.

> **Version:** 2.0.0

## 🚀 Features

- **`memoryAccess_t` class** with a single‐call constructor:
  ```cpp
  memoryAccess_t storage(&SPIFFS);
  ```
- **SD-card type detection** via  
  ```cpp
  storage.type(SD.cardType());
  ```  
  Supports MMC / SDSC / SDHC.
- **I/O benchmark** with  
  ```cpp
  storage.testIO("/benchmark.bin");
  ```  
  Measures raw read/write throughput.
- Core file & directory APIs (all take `const char*` paths):
  - `listDir(const char* path, uint8_t levels)`
  - `createDir(const char* path)`
  - `removeDir(const char* path)`
  - `read(const char* path)`
  - `write(const char* path, const char* message)`
  - `append(const char* path, const char* message)`
  - `rename(const char* oldPath, const char* newPath)`
  - `remove(const char* path)`
- **Low overhead**: C-string parameters instead of `String`.
- **Optional serial debugging** with:
  ```cpp
  #define Debug_Serial_Memory
  ```

## 📁 Usage

### 1. Setup & Initialization

```cpp
#include <Arduino.h>
#include <SPIFFS.h>
#include "initMemory.h"

// #define Debug_Serial_Memory

memoryAccess_t storage(&SPIFFS);

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  // Mount SPIFFS (or LittleFS, SD)
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ SPIFFS mount failed");
    return;
  }
  Serial.println("✅ SPIFFS mounted");
}
```

### 2. SD-Card Type Detection

```cpp
#include <SD.h>

// After `SD.begin(pin);`
uint8_t cardType = SD.cardType();
storage.type(cardType);
```

### 3. Directory & File Operations

```cpp
// Create a folder
storage.createDir("/logs");

// List contents (one level deep)
storage.listDir("/logs", 1);

// Write, read, and append
storage.write("/logs/run.txt", "System start\n");
String data = storage.read("/logs/run.txt");
storage.append("/logs/run.txt", "Heartbeat OK\n");

// Rename and delete
storage.rename("/logs/run.txt", "/logs/old_run.txt");
storage.remove("/logs/old_run.txt");

// Remove directory
storage.removeDir("/logs");
```

### 4. I/O Benchmark

```cpp
// Produce a 1 MB write/read test
storage.testIO("/benchmark.bin");
```

> Tip: Uncomment `#define Debug_Serial_Memory` **before** including `initMemory.h` to view verbose logs.

## 🧰 Dependencies

- **Board**: ESP32 (tested on ESP32 DevKitC)  
- **Libraries**:
  - SPIFFS, LittleFS or SD (Arduino FS API)  

## 📦 Installation

1. **Arduino Library Manager**  
   Search for “initMemory” and install v2.0.0 directly.

2. **Manual install**  
   - Clone or download this repo into your Arduino `libraries/` folder.  
   - Restart the Arduino IDE.  
   - In your sketch:
     ```cpp
     #include <initMemory.h>
     ```

Or install via Library Manager once registered.

## 🧾 License

This project is licensed under the **MIT License** – see [LICENSE](LICENSE) for details.

## ✍️ Author

**Milad Nikpendar**  
GitHub: [milad-nikpendar/initMemory](https://github.com/milad-nikpendar/initMemory)  
Email: milad82nikpendar@gmail.com  
