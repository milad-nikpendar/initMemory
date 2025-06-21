#include "initTransport.h"

// Function to set up the origin file system type, path, and cargo name for the transporter
void transportAccess_t::origin_setup(fs::FS *memory, std::string path, std::string cargo, bool is_dir)
{
    originMemory = memory;                                           // Set the origin file system type
    cargoName = cargo;                                               // Store the name of the file to be transported
    originPath = path + (path.back() == '/' ? "" : "/") + cargoName; // Construct the origin path for the transporter
    is_transport_dir = is_dir;                                       // Set the flag for directory transport
    goalMemory = nullptr;                                            // Reset the goal file system type
    goalPath = "";                                                   // Clear the goal path for the transporter
}

// Function to set up the goal file system type and path for the transporter
void transportAccess_t::goal_setup(fs::FS *memory, std::string path)
{
    goalMemory = memory;                                           // Set the goal file system type
    goalPath = path + (path.back() == '/' ? "" : "/") + cargoName; // Construct the goal path for the transporter
}

// Reset the transporter state
void transportAccess_t::resert_transport()
{
    is_transport_cut = false; // Reset the transporter cut flag
    is_transport_dir = false; // Reset the transporter directory flag
    cargoName = "";           // Clear the cargo name
    originMemory = getFS();   // Reset the origin file system type
    originPath = "";          // Clear the origin path
    goalMemory = getFS();     // Reset the goal file system type
    goalPath = "";            // Clear the goal path
}

// Check if the current file is the origin file for the transporter
bool transportAccess_t::isOrigin(fs::FS *memory, std::string path, std::string cargo, bool is_dir)
{
    std::string userCargo = cargo; // Store the cargo name for the transporter
    std::string userPath = path + (path.back() == '/' ? "" : "/") + cargoName;
    if (memory == originMemory && userPath == originPath && cargo == cargoName && is_dir == is_transport_dir)
        return true; // Check if the current file matches the origin file for the transporter
    return false;    // Return false if it does not match
}

// Check if the current file is the goal file for the transporter
bool transportAccess_t::isGoal(fs::FS *memory, std::string path)
{
    std::string userPath = path + (path.back() == '/' ? "" : "/") + cargoName;
    if (memory == goalMemory && userPath == goalPath)
        return true; // Check if the current file matches the goal file for the transporter
    return false;    // Return false if it does not match
}

// Function to generate a unique name for a file based on the original path
String transportAccess_t::generateUniqueName(String originalPath, fs::FS *memory)
{
    String newName = ""; // Initialize newName as an empty string
    if (!exists(originalPath, memory))
    {
        newName = originalPath; // Start with the original path
    }
    else if (originalPath.indexOf('.') != -1) // Check if the path has an extension
    {
        // Separate the extension
        int dotIndex = originalPath.lastIndexOf('.');
        String baseName = (dotIndex != -1) ? originalPath.substring(0, dotIndex) : originalPath;
        String extension = (dotIndex != -1) ? originalPath.substring(dotIndex) : "";

        baseName += (baseName.indexOf("_") != -1) ? "" : "_"; // Ensure there's an underscore before the number

        // Check if the base name ends with a number
        int lastDigitIndex = baseName.length() - 1;
        while (lastDigitIndex >= 0 && isDigit(baseName[lastDigitIndex]))
        {
            lastDigitIndex--;
        }

        String nameWithoutNumber = baseName.substring(0, lastDigitIndex + 1); // Add an underscore to separate the name from the number
        String numberPart = baseName.substring(lastDigitIndex + 1);

        int number = (numberPart.length() > 0) ? numberPart.toInt() + 1 : 1; // Start with 1 if no number is found
        newName = nameWithoutNumber + String(number) + extension;            // Create the new name with incremented number

        // Check if the new name already exists in the filesystem
        while (exists(newName, memory))
        {
            number++;
            newName = nameWithoutNumber + String(number) + extension;
        }
    }
    else // If the path does not have an extension
    {
        originalPath += (originalPath.indexOf("_") != -1) ? "" : "_"; // Ensure there's an underscore before the number

        // Check if the base name ends with a number
        int lastDigitIndex = originalPath.length() - 1;
        while (lastDigitIndex >= 0 && isDigit(originalPath[lastDigitIndex]))
        {
            lastDigitIndex--;
        }

        String nameWithoutNumber = originalPath.substring(0, lastDigitIndex + 1); // Add an underscore to separate the name from the number
        String numberPart = originalPath.substring(lastDigitIndex + 1);

        int number = (numberPart.length() > 0) ? numberPart.toInt() + 1 : 1; // Start with 1 if no number is found
        newName = nameWithoutNumber + String(number);                        // Create the new name with incremented number

        // Check if the new name already exists in the filesystem
        while (exists(newName, memory))
        {
            number++;
            newName = nameWithoutNumber + String(number);
        }
    }
#ifdef Debug_Serial_Memory
    Serial.printf("Original path: %s\n", originalPath.c_str());    // Debug output
    Serial.printf("Generated unique name: %s\n", newName.c_str()); // Debug output
#endif
    return newName; // Return the unique name
}

// Function to copy a file from source to destination
bool transportAccess_t::copyFile(fs::FS *sourceMemory, std::string sourcePath, fs::FS *destinMemory, std::string destinPath)
{
    if (sourceMemory == nullptr || destinMemory == nullptr)
        return false; // Return false if either memory pointer is null

    File sourceFile = sourceMemory->open(sourcePath.c_str(), "r");
    if (!sourceFile)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Error opening source file!");
#endif
        return false;
    }
    String destinPathStr = generateUniqueName(destinPath.c_str(), destinMemory); // Generate a unique name for the destination file
    File destFile = destinMemory->open(destinPathStr.c_str(), "w");              // Open the destination file for writing
    if (!destFile)
    {
#ifdef Debug_Serial_Memory
        Serial.println("Error opening destination file!");
#endif
        sourceFile.close();
        return false;
    }

    uint8_t buffer[1024]; // Buffer size can be adjusted as needed
    size_t bytesRead;
    while ((bytesRead = sourceFile.read(buffer, sizeof(buffer))) > 0)
    {
        destFile.write(buffer, bytesRead);
        if (refresh_ui)
            refresh_ui(); // Call the UI refresh function if set
    }

    sourceFile.close(); // Close the source file after reading
    destFile.close();   // Close the destination file after writing

    if (is_transport_cut)
    {
        if (!remove(sourcePath.c_str(), sourceMemory)) // If the transport operation was cut, remove the original file
        {
#ifdef Debug_Serial_Memory
            Serial.printf("Error removing original file: %s\n", sourcePath.c_str());
#endif
        }
    } // If the transport operation was cut, remove the original file
#ifdef Debug_Serial_Memory
    Serial.printf("File copied from %s to %s\n", sourcePath.c_str(), destinPathStr.c_str());
#endif
    return true;
}

// Function to copy a folder from source to destination
bool transportAccess_t::copyFolder(fs::FS *sourceMemory, std::string sourcePath, fs::FS *destinMemory, std::string destinPath)
{
    if (sourceMemory == nullptr || destinMemory == nullptr)
        return false; // Return false if either memory pointer is null

    File dir = sourceMemory->open(sourcePath.c_str()); // Open the source directory
    if (!dir || !dir.isDirectory())
    {
#ifdef Debug_Serial_Memory
        Serial.printf("Invalid folder: %s\n", sourcePath.c_str());
#endif
        return false; // Return if the source path is not a directory
    }

    String destinPathStr = generateUniqueName(destinPath.c_str(), destinMemory); // Generate a unique name for the destination directory
    destinMemory->mkdir(destinPathStr.c_str());                                  // Create the destination directory if it does not exist
#ifdef Debug_Serial_Memory
    Serial.printf("Folder created: %s\n", destinPathStr.c_str());
#endif
    File file = dir.openNextFile();
    while (file)
    {
        std::string newsourcePath = std::string(sourcePath) + (sourcePath[strlen(sourcePath.c_str()) - 1] == '/' ? "" : "/") + std::string(file.name());                          // Construct the source path for the transporter
        std::string newdestinPath = std::string(destinPathStr.c_str()) + (destinPathStr.c_str()[strlen(destinPathStr.c_str()) - 1] == '/' ? "" : "/") + std::string(file.name()); // Construct the destination path for the transporter

        if (file.isDirectory())
            copyFolder(sourceMemory, newsourcePath.c_str(), destinMemory, newdestinPath.c_str()); // Recursively copy subdirectory
        else
            copyFile(sourceMemory, newsourcePath.c_str(), destinMemory, newdestinPath.c_str()); // Copy the file from source to destination

        file = dir.openNextFile();
    }
    dir.close(); // Close the directory if it was opened

    if (is_transport_cut)
    {
        if (!removeDir(sourcePath.c_str(), sourceMemory)) // If the transport operation was cut, remove the original directory
        {
#ifdef Debug_Serial_Memory
            Serial.printf("Error removing original folder: %s\n", sourcePath.c_str());
#endif
        }
    }
#ifdef Debug_Serial_Memory
    Serial.printf("Folder copied from %s to %s\n", sourcePath.c_str(), destinPathStr.c_str());
#endif
    return true; // Return true if the folder was copied successfully
}
