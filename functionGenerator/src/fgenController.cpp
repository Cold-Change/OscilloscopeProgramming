#include "fgenController.h"
#include <iostream>

fgenController::fgenController(scpWriter *w, fgenConfig *c, scpWaveformGen *g)
    : writer(w), config(c), waveGen(g), isRunning(false)
{
    std::cout << "[fgenController] Function generator controller initialized" << std::endl;
}

void fgenController::start()
{
    if (isRunning)
    {
        std::cout << "[fgenController] Function generator already running" << std::endl;
        return;
    }
    
    isRunning = true;
    std::cout << "[fgenController] Function generator STARTED" << std::endl;
    std::cout << "[fgenController] Current configuration:" << std::endl;
    config->displayConfig();
}

void fgenController::stop()
{
    if (!isRunning)
    {
        std::cout << "[fgenController] Function generator already stopped" << std::endl;
        return;
    }
    
    isRunning = false;
    std::cout << "[fgenController] Function generator STOPPED" << std::endl;
}

void fgenController::generateAndOutput()
{
    if (!isRunning)
    {
        std::cerr << "[fgenController] ERROR: Cannot generate. Function generator not running." << std::endl;
        std::cerr << "[fgenController] Please use START_FGEN first." << std::endl;
        return;
    }
    
    std::cout << "[fgenController] Generating waveform..." << std::endl;
    std::cout << "[fgenController] Wave Type: " << config->waveTypeToString() << std::endl;
    std::cout << "[fgenController] Frequency: " << config->getFrequency() << " Hz" << std::endl;
    std::cout << "[fgenController] Amplitude: " << config->getAmplitude() << " V" << std::endl;
    std::cout << "[fgenController] Offset: " << config->getOffset() << " V" << std::endl;
    std::cout << "[fgenController] Samples: " << config->getNumSamples() << std::endl;
    
    // Generate waveform based on configuration
    std::vector<uint8_t> waveform = waveGen->generateWaveform();
    
    if (waveform.empty())
    {
        std::cerr << "[fgenController] ERROR: Failed to generate waveform" << std::endl;
        return;
    }
    
    std::cout << "[fgenController] Waveform generated successfully (" 
              << waveform.size() << " samples)" << std::endl;
    
    // Output to file
    std::string outputFile = config->getOutputFile();
    if (!outputFile.empty())
    {
        writer->writeData(outputFile, waveform);
        std::cout << "[fgenController] Waveform written to " << outputFile << std::endl;
    }
    else
    {
        std::cerr << "[fgenController] WARNING: No output file specified" << std::endl;
    }
}

bool fgenController::isActive() const
{
    return isRunning;
}

std::string fgenController::getStatus() const
{
    if (isRunning)
    {
        return "RUNNING - " + config->waveTypeToString() + " @ " + 
               std::to_string(config->getFrequency()) + " Hz";
    }
    else
    {
        return "STOPPED";
    }
}

// NEW METHODS FOR CONFIGURATION

void fgenController::setWaveType(const std::string &typeStr)
{
    WaveType type = fgenConfig::stringToWaveType(typeStr);
    config->setWaveType(type);
}

void fgenController::setFrequency(double freq)
{
    config->setFrequency(freq);
}

void fgenController::setAmplitude(double amp)
{
    config->setAmplitude(amp);
}

void fgenController::setOffset(double offset)
{
    config->setOffset(offset);
}

void fgenController::setNumSamples(int samples)
{
    config->setNumSamples(samples);
}

void fgenController::setOutputFile(const std::string &filename)
{
    config->setOutputFile(filename);
}

void fgenController::displayConfig() const
{
    config->displayConfig();
}