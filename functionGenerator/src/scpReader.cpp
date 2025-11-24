#include "scpReader.h"
#include "scpConfig.h"

scpReader::scpReader(FtdiDevice &device) 
    : dev(device), bufferSize(4096) {}

std::vector<uint8_t> scpReader::readSamples(int numberOfSamples)
{
    if (!dev.isOpen())
    {
        std::cerr << "ERROR: Read device not open." << std::endl;
        return {};
    }

    std::vector<uint8_t> data(numberOfSamples);
    DWORD bytesRead = 0;
    
    FT_HANDLE ftHandle = dev.getHandle();
    
    if (ftHandle == nullptr)
    {
        std::cerr << "[scpReader] ERROR: Invalid FTDI handle" << std::endl;
        return {};
    }
    
    // Purge RX buffer before reading
    FT_STATUS ftStatus = FT_Purge(ftHandle, FT_PURGE_RX);
    if (ftStatus != FT_OK)
    {
        std::cerr << "[scpReader] WARNING: Failed to purge RX buffer (code: " << ftStatus << ")" << std::endl;
    }
    
    // Read data from FTDI device
    ftStatus = FT_Read(ftHandle, data.data(), numberOfSamples, &bytesRead);
    
    if (ftStatus != FT_OK)
    {
        std::cerr << "[scpReader] ERROR: FT_Read failed with status " << ftStatus << std::endl;
        return {};
    }
    
    // Resize to actual bytes read
    data.resize(bytesRead);
    
    std::cout << "[scpReader] " << bytesRead << " samples read from FTDI device" << std::endl;
    
    return data;
}

// NEW: Set baud rate for reading frequency control
void scpReader::setBaudRate(int baudRate)
{
    if (!dev.isOpen())
    {
        std::cerr << "[scpReader] ERROR: Device not open" << std::endl;
        return;
    }
    
    FT_HANDLE ftHandle = dev.getHandle();
    if (ftHandle == nullptr)
    {
        std::cerr << "[scpReader] ERROR: Invalid FTDI handle" << std::endl;
        return;
    }
    
    FT_STATUS ftStatus = FT_SetBaudRate(ftHandle, baudRate);
    
    if (ftStatus == FT_OK)
    {
        std::cout << "[scpReader] ✓ Baud rate set to " << baudRate << " baud" << std::endl;
        std::cout << "[scpReader] ✓ Effective sampling: " << (baudRate * 16) << " Hz" << std::endl;
    }
    else
    {
        std::cerr << "[scpReader] ERROR: Failed to set baud rate (code: " << ftStatus << ")" << std::endl;
    }
}


std::vector<uint8_t> scpReader::readData(const std::string &filename)
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

    std::cout << "[scpReader] " << data.size() << " bytes read from " << filename << std::endl;
    return data;
}

size_t scpReader::getBufferSize() const
{
    return bufferSize;
}

void scpReader::setBufferSize(size_t size)
{
    bufferSize = size;
    std::cout << "[scpReader] Buffer size set to " << bufferSize << " bytes" << std::endl;
}
