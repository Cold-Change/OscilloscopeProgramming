#include "scpConfig.h"
#include <fstream>
#include <sstream>
#include <cmath>

// Initialize static counter
std::atomic<int> scpConfig::nextConfigId(1);

scpConfig::scpConfig() 
    : configId(nextConfigId++),
      sampleRate(1000), 
      numberOfSamples(10000), 
      triggerLevel(0.0), 
      triggerEnabled(false),
      inputFile(""),
      outputFile("output.dat"),
      baudRate(9600),              // Default 9600 baud
      samplingFrequency(153600.0)  // Effective: 9600 * 16 = 153.6 kHz
{
    label = "[scpConfig-" + std::to_string(configId) + "]";
    std::cout << label << " Configuration initialized with defaults" << std::endl;
}

void scpConfig::setSampleRate(int rate)
{
    sampleRate = rate;
    std::cout << label << " Sample rate set to " << sampleRate << " Hz" << std::endl;
}

void scpConfig::setNumberOfSamples(int n)
{
    numberOfSamples = n;
    std::cout << label << " Number of samples set to " << numberOfSamples << std::endl;
}

void scpConfig::setTriggerLevel(double level)
{
    triggerLevel = level;
    std::cout << label << " Trigger level set to " << triggerLevel << " V" << std::endl;
}

void scpConfig::setTriggerEnabled(bool enabled)
{
    triggerEnabled = enabled;
    std::cout << label << " Trigger " << (triggerEnabled ? "enabled" : "disabled") << std::endl;
}

void scpConfig::setInputFile(const std::string &file)
{
    inputFile = file;
    std::cout << label << " Input file set to " << inputFile << std::endl;
}

void scpConfig::setOutputFile(const std::string &file)
{
    outputFile = file;
    std::cout << label << " Output file set to " << outputFile << std::endl;
}

// NEW: Set sampling frequency (FTDI Bit Bang mode)
void scpConfig::setSamplingFrequency(double freqHz)
{
    samplingFrequency = freqHz;
    
    // Calculate required baud rate
    // Effective frequency = Baud Rate × 16 (for Asynchronous Bit Bang)
    // So: Baud Rate = Desired Frequency / 16
    baudRate = static_cast<int>(std::round(freqHz / 16.0));
    
    // Clamp to valid range (300 - 3000000 baud)
    if (baudRate < 300) baudRate = 300;
    if (baudRate > 3000000) baudRate = 3000000;
    
    // Recalculate actual frequency
    samplingFrequency = baudRate * 16.0;
    
    std::cout << label << " Sampling frequency set to " << samplingFrequency << " Hz" << std::endl;
    std::cout << label << " Baud rate: " << baudRate << " baud" << std::endl;
}

void scpConfig::setBaudRate(int baud)
{
    baudRate = baud;
    samplingFrequency = baudRate * 16.0;
    
    std::cout << label << " Baud rate set to " << baudRate << " baud" << std::endl;
    std::cout << label << " Effective frequency: " << samplingFrequency << " Hz" << std::endl;
}

double scpConfig::getEffectiveFrequency() const
{
    return samplingFrequency;
}

int scpConfig::getBaudRate() const
{
    return baudRate;
}

// ... (rest of the existing methods remain the same)

int scpConfig::getSampleRate() const
{
    return sampleRate;
}

int scpConfig::getNumberOfSamples() const
{
    return numberOfSamples;
}

double scpConfig::getTriggerLevel() const
{
    return triggerLevel;
}

bool scpConfig::isTriggerEnabled() const
{
    return triggerEnabled;
}

std::string scpConfig::getInputFile() const
{
    return inputFile;
}

std::string scpConfig::getOutputFile() const
{
    return outputFile;
}

bool scpConfig::loadFromFile(const std::string &filename)
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
            if (key == "sampleRate") sampleRate = std::stoi(value);
            else if (key == "numberOfSamples") numberOfSamples = std::stoi(value);
            else if (key == "triggerLevel") triggerLevel = std::stod(value);
            else if (key == "triggerEnabled") triggerEnabled = (value == "true" || value == "1");
            else if (key == "inputFile") inputFile = value;
            else if (key == "outputFile") outputFile = value;
            else if (key == "baudRate") setBaudRate(std::stoi(value));
            else if (key == "samplingFrequency") setSamplingFrequency(std::stod(value));
        }
    }
    
    std::cout << label << " Configuration loaded from " << filename << std::endl;
    return true;
}

bool scpConfig::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << label << " ERROR: Cannot create config file " << filename << std::endl;
        return false;
    }
    
    file << "# Oscilloscope Configuration - " << label << "\n";
    file << "sampleRate=" << sampleRate << "\n";
    file << "numberOfSamples=" << numberOfSamples << "\n";
    file << "triggerLevel=" << triggerLevel << "\n";
    file << "triggerEnabled=" << (triggerEnabled ? "true" : "false") << "\n";
    file << "inputFile=" << inputFile << "\n";
    file << "outputFile=" << outputFile << "\n";
    file << "baudRate=" << baudRate << "\n";
    file << "samplingFrequency=" << samplingFrequency << "\n";
    
    std::cout << label << " Configuration saved to " << filename << std::endl;
    return true;
}

void scpConfig::displayConfig() const
{
    std::cout << "\n=== Oscilloscope Configuration " << label << " ===" << std::endl;
    std::cout << "Config ID: " << configId << std::endl;
    std::cout << "Sample Rate: " << sampleRate << " Hz" << std::endl;
    std::cout << "Number of Samples: " << numberOfSamples << std::endl;
    std::cout << "Baud Rate: " << baudRate << " baud" << std::endl;
    std::cout << "Effective Sampling Frequency: " << samplingFrequency << " Hz" << std::endl;
    std::cout << "Trigger Level: " << triggerLevel << " V" << std::endl;
    std::cout << "Trigger Enabled: " << (triggerEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Input File: " << (inputFile.empty() ? "(none)" : inputFile) << std::endl;
    std::cout << "Output File: " << outputFile << std::endl;
    std::cout << "==================================\n" << std::endl;
}