#include <iostream>
#include <string>
#include <vector>
#include "../include/FtdiDevice.h"
#include "../include/ReadFile.h"
#include "../include/WriteFile.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <inputFile> <outputFile>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    FtdiDevice readDevice("FTDI_Read");
    FtdiDevice writeDevice("FTDI_Write");

    readDevice.open();
    writeDevice.open();

    ReadFile reader(readDevice);
    WriteFile writer(writeDevice);

    std::vector<uint8_t> data = reader.readData(inputFile);
    if (data.empty())
    {
        std::cerr << "No data read from file. Aborting." << std::endl;
        readDevice.close();
        writeDevice.close();
        return 1;
    }

    writer.writeData(outputFile, data);

    readDevice.close();
    writeDevice.close();

    std::cout << "Driver Test completed successfully." << std::endl;
    return 0;
}
