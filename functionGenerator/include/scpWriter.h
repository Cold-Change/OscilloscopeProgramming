#ifndef SCPWRITER_H
#define SCPWRITER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FtdiDevice.h"

/**
 * Class: scpWriter (CLS-003)
 * Responsibility: Write data to device via FTDI and to files
 * Association: Uses FtdiDevice (1:0..1) - Optional for hardware writes
 */
class scpWriter
{
public:
    explicit scpWriter(FtdiDevice *device = nullptr);  // CHANGED: pointer with default nullptr
    
    // Write data to file (overwrite mode) - NO FTDI required
    void writeData(const std::string &filename, const std::vector<uint8_t> &data);
    
    // Append data to file - NO FTDI required
    void appendData(const std::string &filename, const std::vector<uint8_t> &data);
    
    // Write data to FTDI hardware (requires open device)
    bool writeToHardware(const std::vector<uint8_t> &data);
    
    // Set append mode
    void setAppendMode(bool mode);
    
    // Get current append mode
    bool getAppendMode() const;

private:
    FtdiDevice *dev;  // CHANGED: pointer (can be nullptr)
    bool appendMode;
};

#endif // SCPWRITER_H