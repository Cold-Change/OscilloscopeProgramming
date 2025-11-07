#include "scpWriter.h"

scpWriter::scpWriter(FtdiDevice &device) 
    : dev(device), appendMode(false) {}

void scpWriter::writeData(const std::string &filename, const std::vector<uint8_t> &data)
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

    std::cout << "[scpWriter] " << data.size() << " bytes written to " << filename << std::endl;
}

void scpWriter::appendData(const std::string &filename, const std::vector<uint8_t> &data)
{
    if (!dev.isOpen())
    {
        std::cerr << "ERROR: Write device not open." << std::endl;
        return;
    }

    std::ofstream out(filename, std::ios::binary | std::ios::app);
    if (!out)
    {
        std::cerr << "ERROR: Cannot open output file " << filename << std::endl;
        return;
    }

    out.write(reinterpret_cast<const char *>(data.data()), data.size());
    out.close();

    std::cout << "[scpWriter] " << data.size() << " bytes appended to " << filename << std::endl;
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
