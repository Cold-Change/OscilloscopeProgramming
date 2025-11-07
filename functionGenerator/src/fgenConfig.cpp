#include "fgenConfig.h"
#include <fstream>
#include <sstream>

fgenConfig::fgenConfig()
    : frequency(1000.0),
      amplitude(1.0),
      offset(0.0),
      waveType(WaveType::SINE),
      outputFile("waveform.dat"),
      numSamples(1000)
{
    std::cout << "[fgenConfig] Configuration initialized with defaults" << std::endl;
}

void fgenConfig::setFrequency(double freq)
{
    frequency = freq;
    std::cout << "[fgenConfig] Frequency set to " << frequency << " Hz" << std::endl;
}

void fgenConfig::setAmplitude(double amp)
{
    amplitude = amp;
    std::cout << "[fgenConfig] Amplitude set to " << amplitude << " V" << std::endl;
}

void fgenConfig::setOffset(double off)
{
    offset = off;
    std::cout << "[fgenConfig] Offset set to " << offset << " V" << std::endl;
}

void fgenConfig::setWaveType(WaveType type)
{
    waveType = type;
    std::cout << "[fgenConfig] Wave type set to " << waveTypeToString() << std::endl;
}

void fgenConfig::setOutputFile(const std::string &file)
{
    outputFile = file;
    std::cout << "[fgenConfig] Output file set to " << outputFile << std::endl;
}

void fgenConfig::setNumSamples(int samples)
{
    numSamples = samples;
    std::cout << "[fgenConfig] Number of samples set to " << numSamples << std::endl;
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
    switch (waveType)
    {
        case WaveType::SINE: return "SINE";
        case WaveType::SQUARE: return "SQUARE";
        case WaveType::TRIANGLE: return "TRIANGLE";
        case WaveType::SAWTOOTH: return "SAWTOOTH";
        default: return "UNKNOWN";
    }
}

WaveType fgenConfig::stringToWaveType(const std::string &str)
{
    if (str == "SINE") return WaveType::SINE;
    if (str == "SQUARE") return WaveType::SQUARE;
    if (str == "TRIANGLE") return WaveType::TRIANGLE;
    if (str == "SAWTOOTH") return WaveType::SAWTOOTH;
    return WaveType::SINE; // default
}

bool fgenConfig::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "[fgenConfig] ERROR: Cannot open config file " << filename << std::endl;
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
    
    std::cout << "[fgenConfig] Configuration loaded from " << filename << std::endl;
    return true;
}

bool fgenConfig::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "[fgenConfig] ERROR: Cannot create config file " << filename << std::endl;
        return false;
    }
    
    file << "frequency=" << frequency << "\n";
    file << "amplitude=" << amplitude << "\n";
    file << "offset=" << offset << "\n";
    file << "waveType=" << waveTypeToString() << "\n";
    file << "outputFile=" << outputFile << "\n";
    file << "numSamples=" << numSamples << "\n";
    
    std::cout << "[fgenConfig] Configuration saved to " << filename << std::endl;
    return true;
}

void fgenConfig::displayConfig() const
{
    std::cout << "\n=== Function Generator Configuration ===" << std::endl;
    std::cout << "Frequency: " << frequency << " Hz" << std::endl;
    std::cout << "Amplitude: " << amplitude << " V" << std::endl;
    std::cout << "Offset: " << offset << " V" << std::endl;
    std::cout << "Wave Type: " << waveTypeToString() << std::endl;
    std::cout << "Output File: " << outputFile << std::endl;
    std::cout << "Samples per Waveform: " << numSamples << std::endl;
    std::cout << "========================================\n" << std::endl;
}
