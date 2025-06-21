/*
   initMemory Library V3.0.0
   Created by Milad Nikpendar
*/
#ifndef INIT_MEMORY_H
#define INIT_MEMORY_H

#include <Arduino.h> // Include the Arduino library for basic functionality
#include <FS.h>      // Include the FS library for file system operations
#include <SD.h>      // Include the SD library for SD card operations

#define Debug_Serial_Memory // If defined, sends the performance report to serial

class memoryAccess_t
{
public:
  memoryAccess_t(fs::FS *initMemory) { typeMemory = initMemory; } // Constructor to initialize the memory access with a specific file system
  void changeFS(fs::FS *initMemory) { typeMemory = initMemory; }  // Function to change the file system type
  fs::FS *getFS() { return typeMemory; }                          // Function to get the current file system type
  bool type(uint8_t cardType);                                    // Function to check the type of SD card attached

  // Function to open the file
  File open(const char *path, fs::FS *memory);
  File open(String path, fs::FS *memory) { return open(path.c_str(), memory); }
  File open(const char *path) { return open(path, getFS()); }
  File open(String path) { return open(path.c_str(), getFS()); }

  size_t size(File target); // Function to calculate the size of a file or directory

  bool exists(const char *dirname, fs::FS *memory);
  bool exists(String dirname, fs::FS *memory) { return exists(dirname.c_str(), memory); }
  bool exists(const char *dirname) { return exists(dirname, getFS()); }
  bool exists(String dirname) { return exists(dirname.c_str(), getFS()); }

  // Function to list the contents of a directory
  bool listDir(const char *dirname, uint8_t levels, fs::FS *memory);
  bool listDir(const char *dirname, uint8_t levels) { return listDir(dirname, levels, getFS()); }

  // Function to create a directory
  bool createDir(const char *path, fs::FS *memory);
  bool createDir(const char *path) { return createDir(path, getFS()); }

  // Function to format a directory by removing all files and subdirectories
  bool formatDir(const char *dirname, fs::FS *memory);
  bool formatDir(const char *dirname) { return formatDir(dirname, getFS()); }

  // Function to remove a directory and all its contents
  bool removeDir(const char *path, fs::FS *memory);
  bool removeDir(const char *path) { return removeDir(path, getFS()); }

  // Function to read the entire file and return its content as a String
  String read(const char *path, fs::FS *memory);
  String read(const char *path) { return read(path, getFS()); } // Overloaded function to read a file

  // Function to read lines from the file
  String readLines(File &file, int count_lines, bool is_end);

  // Function to write a message to the file
  bool write(const char *path, const char *message, fs::FS *memory);
  bool write(const char *path, const char *message) { return write(path, message, getFS()); }

  // Function to append a message to the file
  bool append(const char *path, const char *message, fs::FS *memory);
  bool append(const char *path, const char *message) { return append(path, message, getFS()); }

  // Function to move backward in the file
  void cursorLines(File &file, uint32_t tempPos, int move);

  // Renames the file from path1 to path2
  bool rename(const char *path1, const char *path2, fs::FS *memory);
  bool rename(const char *path1, const char *path2) { return rename(path1, path2, getFS()); }

  // Removes the file from the path
  bool remove(const char *path, fs::FS *memory);
  bool remove(const char *path) { return remove(path, getFS()); }

  // Function to test read and write operations on a file
  void testIO(const char *path, fs::FS *memory);
  void testIO(const char *path) { testIO(path, getFS()); }

  // Function to set the UI refresh function pointer
  void set_refresh_ui(void (*func)());

  // Function to format the file system
  bool format(fs::FS *memory);
  bool format() { return format(getFS()); }

private:
  fs::FS *typeMemory = nullptr; // Pointer to the current file system type

  size_t calculateFolderSize(File dir); // Function to format the file system
protected:
  void (*refresh_ui)() = nullptr; // Pointer to the function that refreshes the UI
};

#endif // INIT_MEMORY_H