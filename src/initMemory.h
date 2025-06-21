#pragma once
#include <Arduino.h>
#include <FS.h>

/**
 * @brief A simple file-system helper for ESP32 (SPIFFS, LittleFS, SD).
 * @version 1.0.0
 * @author Milad Nikpendar
 */
struct memory_t {
  memory_t();

  /**
   * @brief Initialize the FS instance (SPIFFS, LittleFS or SD).
   * @param fs Pointer to an initialized fs::FS object.
   */
  void init(fs::FS *fs);

  /**
   * @brief List files and directories.
   * @param dirname Root path to list.
   * @param levels Depth of recursion.
   * @return true on success, false otherwise.
   */
  bool listDir(const String &dirname, uint8_t levels);

  /**
   * @brief Create a directory.
   * @param path Directory path to create.
   * @return true on success.
   */
  bool createDir(const String &path);

  /**
   * @brief Remove a directory.
   * @param path Directory path to remove.
   * @return true on success.
   */
  bool removeDir(const String &path);

  /**
   * @brief Read the entire contents of a text file.
   * @param path File path.
   * @return File contents as String (or error message).
   */
  String read(const String &path);

  /**
   * @brief Write (overwrite) a text file.
   * @param path File path.
   * @param message Content to write.
   * @return true on success.
   */
  bool write(const String &path, const String &message);

  /**
   * @brief Append to a text file.
   * @param path File path.
   * @param message Content to append.
   * @return true on success.
   */
  bool append(const String &path, const String &message);

  /**
   * @brief Rename or move a file.
   * @param path1 Old path.
   * @param path2 New path.
   * @return true on success.
   */
  bool rename(const String &path1, const String &path2);

  /**
   * @brief Delete a file.
   * @param path File path.
   * @return true on success.
   */
  bool remove(const String &path);

private:
  fs::FS *typeMemory;
};
