#include "fgenConfig.h"
#include <fstream>
#include <sstream>

// Initialize static counter
std::atomic<int> fgenConfig::nextConfigId(1);

fgenConfig::fgenConfig()
    : configId(nextConfigId++),
      frequency(1000.0),
      amplitude(1.0),
      offset(0.0),
      waveType(WaveType::SINE),
      outputFile("waveform.dat"),
      numSamples(1000)
{
    label = "[fgenConfig-" + std::to_string(configId) + "]";
    std::cout << label << " Configuration initialized with defaults" << std::endl;
}

void fgenConfig::setFrequency(double freq)
{
    frequency = freq;
    std::cout << label << " Frequency set to " << frequency << " Hz" << std::endl;
}

void fgenConfig::setAmplitude(double amp)
{
    amplitude = amp;
    std::cout << label << " Amplitude set to " << amplitude << " V" << std::endl;
}

void fgenConfig::setOffset(double off)
{
    offset = off;
    std::cout << label << " Offset set to " << offset << " V" << std::endl;
}

void fgenConfig::setWaveType(WaveType type)
{
    waveType = type;
    std::cout << label << " Wave type set to " << waveTypeToStringWithId(type) << std::endl;
}

void fgenConfig::setOutputFile(const std::string &file)
{
    outputFile = file;
    std::cout << label << " Output file set to " << outputFile << std::endl;
}

void fgenConfig::setNumSamples(int samples)
{
    numSamples = samples;
    std::cout << label << " Number of samples set to " << numSamples << std::endl;
}

double fgenConfig::getFrequency() const
{
    return frequency;
}

double fgenConfig::getAmplitude() const
{
    return amplitude;
}

double fgenConfig::getOffset() const
{
    return offset;
}

WaveType fgenConfig::getWaveType() const
{
    return waveType;
}

std::string fgenConfig::getOutputFile() const
{
    return outputFile;
}

int fgenConfig::getNumSamples() const
{
    return numSamples;
}

std::string fgenConfig::waveTypeToString() const
{
    return getWaveTypeLabel(waveType);
}

std::string fgenConfig::waveTypeToStringWithId(WaveType type) const
{
    return "[WaveType-" + std::to_string(getWaveTypeId(type)) + "] " + getWaveTypeLabel(type);
}

WaveType fgenConfig::stringToWaveType(const std::string &str)
{
    if (str == "SINE" || str == "sine" || str == "0") return WaveType::SINE;
    if (str == "SQUARE" || str == "square" || str == "1") return WaveType::SQUARE;
    if (str == "TRIANGLE" || str == "triangle" || str == "2") return WaveType::TRIANGLE;
    if (str == "SAWTOOTH" || str == "sawtooth" || str == "3") return WaveType::SAWTOOTH;
    return WaveType::SINE; // default
}

std::string fgenConfig::getWaveTypeLabel(WaveType type)
{
    switch (type)
    {
        case WaveType::SINE: return "SINE";
        case WaveType::SQUARE: return "SQUARE";
        case WaveType::TRIANGLE: return "TRIANGLE";
        case WaveType::SAWTOOTH: return "SAWTOOTH";
        default: return "UNKNOWN";
    }
}

int fgenConfig::getWaveTypeId(WaveType type)
{
    return static_cast<int>(type);
}

bool fgenConfig::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << label << " ERROR: Cannot open config file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value))
        {
            if (key == "frequency") frequency = std::stod(value);
            else if (key == "amplitude") amplitude = std::stod(value);
            else if (key == "offset") offset = std::stod(value);
            else if (key == "waveType") waveType = stringToWaveType(value);
            else if (key == "outputFile") outputFile = value;
            else if (key == "numSamples") numSamples = std::stoi(value);
        }
    }
    
    std::cout << label << " Configuration loaded from " << filename << std::endl;
    return true;
}

bool fgenConfig::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << label << " ERROR: Cannot create config file " << filename << std::endl;
        return false;
    }
    
    file << "# Function Generator Configuration - " << label << "\n";
    file << "frequency=" << frequency << "\n";
    file << "amplitude=" << amplitude << "\n";
    file << "offset=" << offset << "\n";
    file << "waveType=" << waveTypeToString() << "\n";
    file << "outputFile=" << outputFile << "\n";
    file << "numSamples=" << numSamples << "\n";
    
    std::cout << label << " Configuration saved to " << filename << std::endl;
    return true;
}

void fgenConfig::displayConfig() const
{
    std::cout << "\n=== Function Generator Configuration " << label << " ===" << std::endl;
    std::cout << "Config ID: " << configId << std::endl;
    std::cout << "Frequency: " << frequency << " Hz" << std::endl;
    std::cout << "Amplitude: " << amplitude << " V" << std::endl;
    std::cout << "Offset: " << offset << " V" << std::endl;
    std::cout << "Wave Type: " << waveTypeToStringWithId(waveType) << std::endl;
    std::cout << "Output File: " << outputFile << std::endl;
    std::cout << "Samples per Waveform: " << numSamples << std::endl;
    std::cout << "========================================\n" << std::endl;
}