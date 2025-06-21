# initMemory

A lightweight and simple memory management library for ESP32-based projects, built on top of the Arduino FS interface.

## 📦 Features

- Initialize internal/external file systems (SPIFFS, LittleFS, SD)
- List directory contents recursively
- Create and remove directories
- Read, write, and append to text files
- Rename and delete files
- Optional debug output to `Serial`

## 📁 Usage

### 1. Initialization

```cpp
memory_t memory;
memory.init(&SPIFFS); // or &LittleFS or &SD
```

### 2. Directory Handling

```cpp
memory.createDir("/mydir");
memory.listDir("/", 1);
memory.removeDir("/mydir");
```

### 3. File Operations

```cpp
memory.write("/file.txt", "Hello World");
String data = memory.read("/file.txt");
memory.append("/file.txt", "\nNew line");
memory.rename("/file.txt", "/renamed.txt");
memory.remove("/renamed.txt");
```

> 🔧 To enable debug logs over serial, define `MemorydebugSerial` before including the header:
```cpp
#define MemorydebugSerial
#include "initMemory.h"
```

## 🧰 Dependencies

- Arduino-compatible board (tested with ESP32)
- File system library (SPIFFS, LittleFS, or SD)

## 🧾 License

MIT License – see [LICENSE](LICENSE) for details.

## ✍️ Author

Created with ❤️ by **Milad Nikpendar**

GitHub: [https://github.com/milad-nikpendar](https://github.com/milad-nikpendar)
