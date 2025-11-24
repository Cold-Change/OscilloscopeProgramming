#ifndef SCPREADER_H
#define SCPREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FtdiDevice.h"

class scpReader
{
public:
    explicit scpReader(FtdiDevice &device);
    
    std::vector<uint8_t> readData(const std::string &filename);
    std::vector<uint8_t> readSamples(int numberOfSamples);
    
    size_t getBufferSize() const;
    void setBufferSize(size_t size);
    
    // NEW: Frequency control
    void setBaudRate(int baudRate);

private:
    FtdiDevice &dev;
    size_t bufferSize;
};

#endif // SCPREADER_H