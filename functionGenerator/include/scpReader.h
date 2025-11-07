#ifndef SCPREADER_H
#define SCPREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FtdiDevice.h"

/**
 * Class: scpReader (CLS-002)
 * Responsibility: Read data from oscilloscope via FTDI device
 * Association: Uses FtdiDevice (1:1)
 */
class scpReader
{
public:
    explicit scpReader(FtdiDevice &device);
    
    // Read data from file
    std::vector<uint8_t> readData(const std::string &filename);
    
    // Read specified number of samples from device
    std::vector<uint8_t> readSamples(int numberOfSamples);
    
    // Get current buffer size
    size_t getBufferSize() const;
    
    // Set buffer size for reading
    void setBufferSize(size_t size);

private:
    FtdiDevice &dev;
    size_t bufferSize;
};

#endif // SCPREADER_H
