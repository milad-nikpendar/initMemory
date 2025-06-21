#include "initMemory.h"

bool memoryAccess_t::type(uint8_t cardType)
{
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    switch (cardType)
    {
    case CARD_MMC:
        Serial.println("MMC");
        break;
    case CARD_SD:
        Serial.println("SDSC");
        break;
    case CARD_SDHC:
        Serial.println("SDHC");
        break;
    default:
        Serial.println("UNKNOWN");
        break;
    }
    return true;
}

// Function to open the file
File memoryAccess_t::open(const char *path, fs::FS *memory)
{
    File file = memory->open(path);
    return file;
}

size_t memoryAccess_t::size(File target)
{
    return (target.isDirectory() ? (calculateFolderSize(target)) : (target.size()));
}

bool memoryAccess_t::exists(const char *dirname, fs::FS *memory)
{
    return memory->exists(dirname);
}

// Function to list the contents of a directory
bool memoryAccess_t::listDir(const char *dirname, uint8_t levels, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Listing directory: %s\n", dirname);
#endif
    File root = memory->open(dirname);
    if (!root)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Failed to open directory");
#endif
        return false;
    }
    if (!root.isDirectory())
    {
#ifdef Debug_Serial_Memory
        Serial.println("Not a directory");
#endif
        return false;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
#ifdef Debug_Serial_Memory
            Serial.print("  DIR : ");
            Serial.println(file.name());
#endif

            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
#ifdef Debug_Serial_Memory
            Serial.print("FILE: ");
            Serial.print(file.name());
            Serial.print(" \tSIZE: ");
            Serial.println(file.size());
#endif
        }
        if (refresh_ui)
            refresh_ui(); // Call the UI refresh function if set
        file = root.openNextFile();
    }
    return true;
}

// Function to check if a file exists
bool memoryAccess_t::createDir(const char *path, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Creating Dir: %s\n", path);
#endif
    if (memory->mkdir(path))
    {
#ifdef Debug_Serial_Memory
        Serial.println("Dir created");
#endif
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("mkdir failed");
#endif
        return false;
    }
}

// Function to format a directory by removing all files and subdirectories
bool memoryAccess_t::formatDir(const char *dirname, fs::FS *memory)
{
    File root = memory->open(dirname);
    if (!root)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Failed to open directory");
#endif
        return false;
    }
    if (!root.isDirectory())
    {
#ifdef Debug_Serial_Memory
        Serial.println("Not a directory");
#endif
        return false;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
#ifdef Debug_Serial_Memory
            Serial.print("  DIR : ");
            Serial.println(file.name());
#endif
            String levelDir = String(dirname) + "/" + String(file.name());
            file.close();
            removeDir(levelDir.c_str()); // Recursively remove subdirectory
        }
        else
        {
#ifdef Debug_Serial_Memory
            Serial.print("FILE: ");
            Serial.print(file.name());
            Serial.print(" \tSIZE: ");
            Serial.println(file.size());
#endif
            String levelDir = String(dirname) + "/" + String(file.name());
            file.close();
            remove(levelDir.c_str()); // Remove the file
        }
        if (refresh_ui)
            refresh_ui(); // Call the UI refresh function if set
        file = root.openNextFile();
    }
    return true;
}

// Function to remove a file
bool memoryAccess_t::removeDir(const char *path, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Removing Dir: %s\n", path);
#endif
    formatDir(path, memory);
    if (memory->rmdir(path))
    {
#ifdef Debug_Serial_Memory
        Serial.println("Dir removed");
#endif
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("rmdir failed");
#endif
        return false;
    }
}

// Function to read the entire file and return its content as a String
String memoryAccess_t::read(const char *path, fs::FS *memory)
{
    String dataStr = "";
#ifdef Debug_Serial_Memory
    Serial.printf("Reading file: %s\n", path);
#endif

    File file = memory->open(path);
    if (!file)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Failed to open file for reading");
#endif
        return "Failed to open file";
    }
#ifdef Debug_Serial_Memory
    Serial.print("Read from file: ");
#endif
    while (file.available())
    {
        char data = file.read();
        dataStr += String(data);
#ifdef Debug_Serial_Memory
        Serial.write(data);
#endif
    }
    file.close();
    return dataStr;
}

// Function to read lines from the file
String memoryAccess_t::readLines(File &file, int count_lines, bool is_end)
{
    String dataStr = "";
    int lineCount = 0;

    while (file.available() && lineCount < count_lines)
    {
        dataStr += file.readStringUntil('\n');
        lineCount++;
    }
    is_end = (!file.available()) ? true : false; // Check if the end of the file is reached
    return dataStr;
}

// Function to write a message to the file
bool memoryAccess_t::write(const char *path, const char *message, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Writing file: %s\n", path);
#endif

    File file = memory->open(path, FILE_WRITE);
    if (!file)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Failed to open file for writing");
#endif
        return false;
    }
    if (file.print(message))
    {
#ifdef Debug_Serial_Memory
        Serial.println("File written");
#endif
        file.close();
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("Write failed");
#endif
        file.close();
        return false;
    }
}

// Function to append a message to the file
bool memoryAccess_t::append(const char *path, const char *message, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Appending to file: %s\n", path);
#endif

    File file = memory->open(path, FILE_APPEND);
    if (!file)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Failed to open file for appending");
#endif
        return false;
    }
    if (file.print(message))
    {
#ifdef Debug_Serial_Memory
        Serial.println("Message appended");
#endif
        file.close();
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("Append failed");
#endif
        file.close();
        return false;
    }
}

// Function to move backward in the file
void memoryAccess_t::cursorLines(File &file, uint32_t tempPos, int move)
{
    // If move is zero, keep the current file position unchanged
    if (move == 0)
    {
        file.seek(tempPos);
        return;
    }

    int lineCount = 0;

    // If move is positive, move forward in the file
    if (move > 0)
    {
        while (file.available() && lineCount < move)
        {
            file.seek(tempPos);
            char ch = file.read();
            tempPos++;

            if (ch == '\n')
            {
                lineCount++;
            }
        }
    }
    else
    {
        int totalMove = abs(move) + 1;
        while (tempPos > 0 && lineCount < totalMove)
        {
            tempPos--;
            file.seek(tempPos);
            char ch = file.peek();

            if (ch == '\n')
            {
                lineCount++;
            }
        }

        // Prevent sudden jump to the beginning of the file
        if (tempPos == 0)
        {
            file.seek(0);
        }
        else
        {
            tempPos++; // Adjust position to avoid cutting off the first character
        }
    }

    // Set the new file position
    file.seek(tempPos);
}

// Renames the file from path1 to path2
bool memoryAccess_t::rename(const char *path1, const char *path2, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Renaming file %s to %s\n", path1, path2);
#endif

    if (memory->rename(path1, path2))
    {
#ifdef Debug_Serial_Memory
        Serial.println("File renamed");
#endif
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("Rename failed");
#endif
        return false;
    }
}

// Removes the file from the path
bool memoryAccess_t::remove(const char *path, fs::FS *memory)
{
#ifdef Debug_Serial_Memory
    Serial.printf("Deleting file: %s\n", path);
#endif

    if (memory->remove(path))
    {
#ifdef Debug_Serial_Memory
        Serial.println("File deleted");
#endif
        return true;
    }
    else
    {
#ifdef Debug_Serial_Memory
        Serial.println("Delete failed");
#endif
        return false;
    }
}

// Function to test read and write operations on a file
void memoryAccess_t::testIO(const char *path, fs::FS *memory)
{
    static uint8_t buf[512];
    size_t len = 0;
    File file = memory->open(path, FILE_WRITE);

    size_t i;
    Serial.print("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
            Serial.print(".");
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    Serial.printf(" - write speed: %u byte/ms\r\n", 2048 * 512 / end);
    file.close();

    file = memory->open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
                toRead = 512;
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
                Serial.print(".");
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        Serial.printf(" - read speed: %u byte/ms\r\n", flen / end);
        file.close();
    }
    else
        Serial.println("- failed to open file for reading");
}
void memoryAccess_t::set_refresh_ui(void (*func)())
{
    refresh_ui = func; // Set the function pointer to refresh the UI
}

// Function to format the file system
size_t memoryAccess_t::calculateFolderSize(File dir)
{
    size_t totalSize = 0;

    while (true)
    {
        File root = dir.openNextFile();
        if (!root)
            break; // End of directory

        if (refresh_ui)
            refresh_ui(); // Call the UI refresh function if set

        if (root.isDirectory())
        {
            totalSize += calculateFolderSize(root); // Recursive call for subdirectory
        }
        else
        {
            totalSize += root.size(); // Add file size
        }
        root.close();
    }
    return totalSize;
}