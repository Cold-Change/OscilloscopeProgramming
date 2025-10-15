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
    explicit ReadFile(FtdiDevice &device) : dev(device) {}

    std::vector<uint8_t> readData(const std::string &filename)
    {
        if (!dev.isOpen())
        {
            std::cerr << "ERROR: Read device not open." << std::endl;
            return {};
        }

        std::ifstream in(filename, std::ios::binary);
        if (!in)
        {
            std::cerr << "ERROR: Cannot open input file " << filename << std::endl;
            return {};
        }

        std::vector<uint8_t> data(
            (std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());

        std::cout << "[READ] " << data.size()
                  << " bytes read from " << filename << std::endl;
        return data;
    }

private:
    FtdiDevice &dev;
};

#endif
