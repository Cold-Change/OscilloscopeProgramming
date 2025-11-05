#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FtdiDevice.h"

class ReadFile
{
public:
    explicit ReadFile(FtdiDevice &device);
    std::vector<uint8_t> readData(const std::string &filename);

private:
    FtdiDevice &dev;
};

#endif
