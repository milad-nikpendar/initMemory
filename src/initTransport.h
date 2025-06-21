/*
   initTransport Library V1.0.0
   Created by Milad Nikpendar
*/
#ifndef INIT_TRANSPORT_H
#define INIT_TRANSPORT_H
#include <Arduino.h>    // Include the Arduino library for basic functionality
#include <FS.h>         // Include the FS library for file system operations
#include <string>       // Include the string library for string manipulation
#include "initMemory.h" // Include the memory access library for file management

class transportAccess_t : public memoryAccess_t
{
public:
    transportAccess_t(fs::FS *initMemory) : memoryAccess_t(initMemory) {}                // Constructor to initialize the transporter with a specific file system type
    void origin_setup(fs::FS *memory, std::string path, std::string cargo, bool is_dir); // Set the origin file system type, path, and cargo name for the transporter
    void goal_setup(fs::FS *memory, std::string path);                                   // Set the goal file system type and path for the transporter
    void set_transport_cut(bool cut) { is_transport_cut = cut; }                         // Set the transport cut flag to delete the original file after copying
    void resert_transport();                                                             // Reset the transporter state

    bool isOrigin(fs::FS *memory, std::string path, std::string cargo, bool is_dir); // Check if the current file is the origin file for the transporter
    bool isGoal(fs::FS *memory, std::string path);                                   // Check if the current file is the goal file for the transporter

    bool copyFile() { return copyFile(originMemory, originPath, goalMemory, goalPath); }     // Function to copy a file from source to destination
    bool copyFolder() { return copyFolder(originMemory, originPath, goalMemory, goalPath); } // Function to copy a folder from source to destination

    bool isTransportCut() const { return is_transport_cut; } // Check if the transport operation is cut
    bool isTransportDir() const { return is_transport_dir; } // Check if the current file is a directory
private:
    bool is_transport_cut = false;  // Flag to check if the transport operation is cut
    bool is_transport_dir = false;  // Flag to check if the current file is a directory
    std::string cargoName;          // Name of the file to be transported
    fs::FS *originMemory = nullptr; // Pointer to the original file system type
    std::string originPath;         // Store the original path for file transporter
    fs::FS *goalMemory = nullptr;   // Pointer to the goal file system type
    std::string goalPath;           // Store the goal path for file transporter

    bool copyFile(fs::FS *sourceMemory, std::string sourcePath, fs::FS *destinMemory, std::string destinPath);   // Function to copy a file from source to destination
    bool copyFolder(fs::FS *sourceMemory, std::string sourcePath, fs::FS *destinMemory, std::string destinPath); // Function to copy a folder from source to destination
protected:
    String generateUniqueName(String originalPath, fs::FS *memory); // Function to generate a unique name for a file based on the original path
};
#endif // INIT_TRANSPORT_H