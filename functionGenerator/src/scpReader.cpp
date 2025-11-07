#include "scpReader.h"

scpReader::scpReader(FtdiDevice &device) 
    : dev(device), bufferSize(4096) {}

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

std::vector<uint8_t> scpReader::readSamples(int numberOfSamples)
{
    if (!dev.isOpen())
    {
        std::cerr << "ERROR: Read device not open." << std::endl;
        return {};
    }

    // Simulate reading numberOfSamples from the FTDI device
    std::vector<uint8_t> data(numberOfSamples);
    
    // In real implementation, this would read from actual hardware
    // For now, we'll fill with simulated data
    for (int i = 0; i < numberOfSamples; ++i)
    {
        data[i] = static_cast<uint8_t>(i % 256);
    }

    std::cout << "[scpReader] " << numberOfSamples << " samples collected from device" << std::endl;
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
