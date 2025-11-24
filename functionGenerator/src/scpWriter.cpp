#include "scpWriter.h"

scpWriter::scpWriter(FtdiDevice *device) 
    : dev(device), appendMode(false) 
{
    if (dev != nullptr) {
        std::cout << "[scpWriter] Writer initialized with FTDI device" << std::endl;
    } else {
        std::cout << "[scpWriter] Writer initialized (file-only mode, no FTDI)" << std::endl;
    }
}

void scpWriter::writeData(const std::string &filename, const std::vector<uint8_t> &data)
{
    // ALWAYS write to file regardless of FTDI connection
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out)
    {
        std::cerr << "[scpWriter] ERROR: Cannot open output file " << filename << std::endl;
        return;
    }

    out.write(reinterpret_cast<const char *>(data.data()), data.size());
    out.close();

    std::cout << "[scpWriter] ✓ " << data.size() << " bytes written to file: " << filename << std::endl;
    
    // Optionally write to FTDI hardware if available
    if (dev != nullptr && dev->isOpen())
    {
        writeToHardware(data);
    }
}

void scpWriter::appendData(const std::string &filename, const std::vector<uint8_t> &data)
{
    std::ofstream out(filename, std::ios::binary | std::ios::app);
    if (!out)
    {
        std::cerr << "[scpWriter] ERROR: Cannot open output file " << filename << std::endl;
        return;
    }

    out.write(reinterpret_cast<const char *>(data.data()), data.size());
    out.close();

    std::cout << "[scpWriter] ✓ " << data.size() << " bytes appended to file: " << filename << std::endl;
}

bool scpWriter::writeToHardware(const std::vector<uint8_t> &data)
{
    if (dev == nullptr)
    {
        std::cerr << "[scpWriter] WARNING: No FTDI device available for hardware write" << std::endl;
        return false;
    }
    
    if (!dev->isOpen())
    {
        std::cerr << "[scpWriter] ERROR: FTDI device not open" << std::endl;
        return false;
    }
    
    FT_HANDLE ftHandle = dev->getHandle();
    
    if (ftHandle == nullptr)
    {
        std::cerr << "[scpWriter] ERROR: Invalid FTDI handle" << std::endl;
        return false;
    }
    
    DWORD bytesWritten = 0;
    FT_STATUS ftStatus = FT_Write(ftHandle, 
                                   const_cast<uint8_t*>(data.data()), 
                                   data.size(), 
                                   &bytesWritten);
    
    if (ftStatus == FT_OK)
    {
        std::cout << "[scpWriter] ✓ " << bytesWritten << " bytes written to FTDI hardware" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "[scpWriter] ERROR: FT_Write failed with status " << ftStatus << std::endl;
        return false;
    }
}

void scpWriter::setAppendMode(bool mode)
{
    appendMode = mode;
    std::cout << "[scpWriter] Append mode " << (appendMode ? "enabled" : "disabled") << std::endl;
}

bool scpWriter::getAppendMode() const
{
    return appendMode;
}