#include <iostream>
#include <string>
#include <vector>
#include "FtdiDevice.h"
#include "ReadFile.cpp"
#include "WriteFile.cpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <inputFile> <outputFile>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    // Instantiate two FTDI devices
    FtdiDevice readDevice("FTDI_Read");
    FtdiDevice writeDevice("FTDI_Write");

    // Open both devices
    readDevice.open();
    writeDevice.open();

    // Create handler objects
    ReadFile reader(readDevice);
    WriteFile writer(writeDevice);

    // Read from file
    std::vector<uint8_t> data = reader.readData(inputFile);
    if (data.empty())
    {
        std::cerr << "No data read from file. Aborting." << std::endl;
        readDevice.close();
        writeDevice.close();
        return 1;
    }

    // Write to file
    writer.writeData(outputFile, data);

    // Close devices
    readDevice.close();
    writeDevice.close();

    std::cout << "Driver Test completed successfully." << std::endl;
    return 0;
}
