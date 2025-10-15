#include <iostream>
#include <vector>
#include <string>
#include "ReadFile.cpp"
#include "WriteFile.cpp"

// ---------------- Command-Line Test Driver ----------------
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <inputfile> <outputfile>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    // Two FTDI devices
    FtdiDevice readDev("FTDI_Read");
    FtdiDevice writeDev("FTDI_Write");
    readDev.open();
    writeDev.open();

    // Create reqfRead and reqfWrite objects
    ReadFile reader(readDev);
    WriteFile writer(writeDev);

    // Read data from input file
    std::vector<uint8_t> data = reader.readData(inputFile);

    // Write data to output file
    writer.writeData(outputFile, data);

    readDev.close();
    writeDev.close();

    std::cout << "[Driver] Test completed successfully.\n";
    return 0;
}
