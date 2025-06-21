# initMemory  v1.0.0

A lightweight and simple memory management library for ESP32-based projects, built on top of the Arduino FS interface.

> **Version:** 1.0.0

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
#include "initMemory.h"

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

> 🔧 To enable serial debug logs, define `MemorydebugSerial` **before** including the header:
> ```cpp
> #define MemorydebugSerial
> #include "initMemory.h"
> ```

## 🧰 Dependencies

- Arduino-compatible board (tested on ESP32)  
- Arduino FS library (SPIFFS, LittleFS, or SD)  

## 🧾 License

MIT License – see [LICENSE](LICENSE) for details.

## ✍️ Author

Created with ❤️ by **Milad Nikpendar**  
GitHub: [milad-nikpendar/initMemory](https://github.com/milad-nikpendar/initMemory)  
```
