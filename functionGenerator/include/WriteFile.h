#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "FtdiDevice.h"

class WriteFile
{
public:
    explicit WriteFile(FtdiDevice &device);
    void writeData(const std::string &filename, const std::vector<uint8_t> &data);

private:
    FtdiDevice &dev;
};

#endif
