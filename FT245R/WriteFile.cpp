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
    explicit WriteFile(FtdiDevice &device) : dev(device) {}

    void writeData(const std::string &filename, const std::vector<uint8_t> &data)
    {
        if (!dev.isOpen())
        {
            std::cerr << "ERROR: Write device not open." << std::endl;
            return;
        }

        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out)
        {
            std::cerr << "ERROR: Cannot open output file " << filename << std::endl;
            return;
        }

        out.write(reinterpret_cast<const char *>(data.data()), data.size());
        out.close();

        std::cout << "[WRITE] " << data.size()
                  << " bytes written to " << filename << std::endl;
    }

private:
    FtdiDevice &dev;
};

#endif
