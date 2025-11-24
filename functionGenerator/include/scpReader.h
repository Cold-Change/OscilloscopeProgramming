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
<<<<<<< Updated upstream
    void setBufferSize(size_t size);
    
    // NEW: Frequency control
    void setBaudRate(int baudRate);
=======
    
<<<<<<< Updated upstream
    // Set buffer size for reading
    void setBufferSize(size_t size);
=======
    // Frequency control
    void setBaudRate(int baudRate);
    
    // NEW: Pin inversion for pull-up compensation
    void setInvertPins(bool invert);
>>>>>>> Stashed changes
>>>>>>> Stashed changes

private:
    FtdiDevice &dev;
    size_t bufferSize;
    bool invertPins;  // NEW: Invert pin logic
};

#endif // SCPREADER_H