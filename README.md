```markdown
# initMemory & initTransport v3.0.0

Version 3.0.0 brings dynamic backend switching and unified file-and-folder management with streaming UI feedback, plus a companion transport layer that handles cross-filesystem copy/move and conflict resolution seamlessly.

> **Version:** 3.0.0  
> **Author:** Milad Nikpendar

---

## Table of Contents

1. [Features](#features)  
2. [Installation](#installation)  
3. [Quickstart](#quickstart)  
4. [API Overview](#api-overview)  
   - [memoryAccess_t (Core FS)](#memoryaccess_t-core-fs)  
   - [transportAccess_t (File Transfer)](#transportaccess_t-file-transfer)  
5. [Examples](#examples)  
6. [Migration Guide](#migration-guide)  
7. [Dependencies](#dependencies)  
8. [License](#license)

---

## Features

### ✨ Core FS (`memoryAccess_t`)

- **Dynamic FS binding** (SPIFFS, LittleFS, SD, RAMDisk…)  
- **FS switching at runtime**: `changeFS()` & `getFS()`  
- **Unified C-string API** (no `String` overhead) for:
  - file open/read/write/append  
  - directory list/create/format/remove  
  - existence checks & size queries (files & folders)  
- **Advanced utilities**:  
  - `formatDir()` — wipe a folder’s contents  
  - `readLines()` & `cursorLines()` — stream lines with UI callbacks  
  - **I/O benchmark** `testIO()` for read/write throughput  
  - **FS format** `format()` for SPIFFS/LittleFS  
- **Optional debug logs** via `#define Debug_Serial_Memory`

### 🔄 Transport (`transportAccess_t`)

- **Inherits** `memoryAccess_t` — full FS API  
- **File/Folder copy** with optional “cut” (move) semantics  
- **Origin/Goal setup** helpers: define source/destination FS, paths, and cargo  
- **Unique name generation** to avoid overwrites  
- **State reset** & validation (`isOrigin()`, `isGoal()`)

---

## Installation

You can install via **PlatformIO** or manually in Arduino IDE:

### PlatformIO

```ini
[env:esp32dev]
platform = espressif32
framework = arduino
lib_deps = 
    milad-nikpendar/initMemory @ ^3.0.0
```

### Arduino IDE / CLI (Manual)

1. Clone this repository into your Arduino `libraries/` folder:  
   ```bash
   git clone https://github.com/milad-nikpendar/initMemory.git \
     ~/Arduino/libraries/initMemory
   ```
2. Restart the Arduino IDE.  
3. Include headers in your sketch:
   ```cpp
   #include <initMemory.h>
   #include <initTransport.h>
   ```

---

## Quickstart

```cpp
#include <Arduino.h>
#include <SPIFFS.h>
#define Debug_Serial_Memory
#include "initMemory.h"
#include "initTransport.h"

// 1. Bind to SPIFFS
memoryAccess_t fs(&SPIFFS);

// 2. Mount & format if needed
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ SPIFFS mount failed");
    return;
  }
  Serial.println("✅ SPIFFS mounted");

  // 3. Basic file ops
  fs.createDir("/data");
  fs.write("/data/hello.txt", "Hello v3!");
  Serial.println(fs.read("/data/hello.txt"));

  // 4. Benchmark
  fs.testIO("/data/bench.bin");

  // 5. File transfer: copy /data/hello.txt → /copy/hello.txt on SD
  SD.begin(5);
  transportAccess_t tx(&SPIFFS);
  tx.origin_setup(&SPIFFS, "/data/hello.txt", "hello.txt", false);
  tx.goal_setup(&SD, "/copy/hello.txt");
  tx.copyFile();
}

void loop() {
  // ...
}
```

---

## API Overview

### memoryAccess_t (Core FS)

| Method                                | Description                                           |
|---------------------------------------|-------------------------------------------------------|
| `memoryAccess_t(fs::FS *backend)`     | Constructor: bind to SPIFFS, SD, LittleFS…            |
| `changeFS(fs::FS *backend)`           | Switch active FS at runtime                           |
| `File open(const char* path)`         | Open a file for read/write                            |
| `size(File file)`                     | Get size of file or directory (recursive)             |
| `exists(const char* path)`            | File/directory existence check                        |
| `listDir(const char* dir, uint8_t lv)`| Recursive directory listing                           |
| `createDir(const char* path)`         | Create a directory                                     |
| `formatDir(const char* dir)`          | Wipe all contents of a directory                      |
| `removeDir(const char* path)`         | Remove directory and contents                         |
| `read(const char* path)`              | Read entire file into `String`                        |
| `readLines(File &f, int n, bool end)` | Read up to `n` lines from file                        |
| `write(const char* p, const char* msg)`| Overwrite or create file                             |
| `append(const char* p, const char* msg)`| Append to file                                       |
| `cursorLines(File &f, uint32_t pos, int mv)`| Seek by lines with UI refresh                 |
| `rename(const char* o, const char* n)`| Rename file                                           |
| `remove(const char* path)`            | Delete file                                           |
| `testIO(const char* path)`            | Benchmark read/write speed                            |
| `set_refresh_ui(void (*fn)())`        | UI refresh callback during long ops                   |
| `format()`                            | Format SPIFFS/LittleFS                                |

### transportAccess_t (File Transfer)

| Method                                                           | Description                                                |
|------------------------------------------------------------------|------------------------------------------------------------|
| `transportAccess_t(fs::FS *backend)`                             | Constructor: bind source FS                                 |
| `origin_setup(fs::FS*, std::string p, std::string cargo, bool)`  | Define source path/cargo (file or dir)                      |
| `goal_setup(fs::FS*, std::string path)`                          | Define destination FS & path                                |
| `set_transport_cut(bool cut)`                                    | Enable “cut” (move) instead of copy                         |
| `copyFile()` / `copyFolder()`                                    | Perform file or folder transfer                             |
| `isOrigin()` / `isGoal()`                                        | Validate origin/goal match                                  |
| `resert_transport()`                                             | Reset internal state                                        |

---

## Examples

- **FS_Explorer** (SPIFFS directory & file demo)  
- **SD_BenchLogger** (SD logging + benchmark)  
- **TransportDemo** (copy/move files between SPIFFS & SD)

Find them under the `/examples` folder, each in its own subdirectory.

---

## Migration Guide from v2.x

1. **Class rename**:  
   - v2: `memoryAccess_t`  
   - v3: still `memoryAccess_t` but expanded—no constructor change.  
2. **FS binding**:  
   - v2: `memoryAccess_t mem(&SPIFFS);`  
   - v3: same, plus `mem.changeFS(&SD);` to swap FS.  
3. **New methods**: replace custom hacks with `formatDir()`, `size()`, `format()`, and `set_refresh_ui()`.  
4. **Transport**: move file operations into `transportAccess_t`—no more manual copy loops.

---

## Dependencies

- **Board**: ESP32 (DevKitC or similar)  
- **Framework**: Arduino  
- **Libraries**:  
  - FS (SPIFFS/LittleFS)  
  - SD (for SD-card support)

---

## 🧾 License

This project is licensed under the **MIT License** – see [LICENSE](LICENSE) for details.

## ✍️ Author

**Milad Nikpendar**  
GitHub: [milad-nikpendar/initMemory](https://github.com/milad-nikpendar/initMemory)  
Email: milad82nikpendar@gmail.com  
