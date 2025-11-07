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
 * Responsibility: Write data to device via FTDI
 * Association: Uses FtdiDevice (1:1)
 */
class scpWriter
{
public:
    explicit scpWriter(FtdiDevice &device);
    
    // Write data to file (overwrite mode)
    void writeData(const std::string &filename, const std::vector<uint8_t> &data);
    
    // Append data to file
    void appendData(const std::string &filename, const std::vector<uint8_t> &data);
    
    // Set append mode
    void setAppendMode(bool mode);
    
    // Get current append mode
    bool getAppendMode() const;

private:
    FtdiDevice &dev;
    bool appendMode;
};

#endif // SCPWRITER_H
