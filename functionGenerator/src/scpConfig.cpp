#include "scpConfig.h"
#include <fstream>
#include <sstream>

scpConfig::scpConfig() 
    : sampleRate(1000), 
      numberOfSamples(10000), 
      triggerLevel(0.0), 
      triggerEnabled(false),
      inputFile(""),
      outputFile("output.dat")
{
    std::cout << "[scpConfig] Configuration initialized with defaults" << std::endl;
}

void scpConfig::setSampleRate(int rate)
{
    sampleRate = rate;
    std::cout << "[scpConfig] Sample rate set to " << sampleRate << " Hz" << std::endl;
}

void scpConfig::setNumberOfSamples(int n)
{
    numberOfSamples = n;
    std::cout << "[scpConfig] Number of samples set to " << numberOfSamples << std::endl;
}

void scpConfig::setTriggerLevel(double level)
{
    triggerLevel = level;
    std::cout << "[scpConfig] Trigger level set to " << triggerLevel << " V" << std::endl;
}

void scpConfig::setTriggerEnabled(bool enabled)
{
    triggerEnabled = enabled;
    std::cout << "[scpConfig] Trigger " << (triggerEnabled ? "enabled" : "disabled") << std::endl;
}

void scpConfig::setInputFile(const std::string &file)
{
    inputFile = file;
    std::cout << "[scpConfig] Input file set to " << inputFile << std::endl;
}

void scpConfig::setOutputFile(const std::string &file)
{
    outputFile = file;
    std::cout << "[scpConfig] Output file set to " << outputFile << std::endl;
}

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
        std::cerr << "[scpConfig] ERROR: Cannot open config file " << filename << std::endl;
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
        }
    }
    
    std::cout << "[scpConfig] Configuration loaded from " << filename << std::endl;
    return true;
}

bool scpConfig::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "[scpConfig] ERROR: Cannot create config file " << filename << std::endl;
        return false;
    }
    
    file << "sampleRate=" << sampleRate << "\n";
    file << "numberOfSamples=" << numberOfSamples << "\n";
    file << "triggerLevel=" << triggerLevel << "\n";
    file << "triggerEnabled=" << (triggerEnabled ? "true" : "false") << "\n";
    file << "inputFile=" << inputFile << "\n";
    file << "outputFile=" << outputFile << "\n";
    
    std::cout << "[scpConfig] Configuration saved to " << filename << std::endl;
    return true;
}

void scpConfig::displayConfig() const
{
    std::cout << "\n=== Oscilloscope Configuration ===" << std::endl;
    std::cout << "Sample Rate: " << sampleRate << " Hz" << std::endl;
    std::cout << "Number of Samples: " << numberOfSamples << std::endl;
    std::cout << "Trigger Level: " << triggerLevel << " V" << std::endl;
    std::cout << "Trigger Enabled: " << (triggerEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Input File: " << (inputFile.empty() ? "(none)" : inputFile) << std::endl;
    std::cout << "Output File: " << outputFile << std::endl;
    std::cout << "==================================\n" << std::endl;
}
