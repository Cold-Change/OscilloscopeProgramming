#include "scpController.h"
#include <iostream>

scpController::scpController(scpReader *r, scpWriter *w, scpConfig *c)
    : reader(r), writer(w), config(c), isRunning(false), collectedSamples(0)
{
    std::cout << "[scpController] Oscilloscope controller initialized" << std::endl;
}

void scpController::start()
{
    if (isRunning)
    {
        std::cout << "[scpController] Oscilloscope already running" << std::endl;
        return;
    }
    
    isRunning = true;
    collectedSamples = 0;
    dataBuffer.clear();
    
    std::cout << "[scpController] Oscilloscope STARTED" << std::endl;
}

void scpController::stop()
{
    if (!isRunning)
    {
        std::cout << "[scpController] Oscilloscope already stopped" << std::endl;
        return;
    }
    
    isRunning = false;
    
    std::cout << "[scpController] Oscilloscope STOPPED" << std::endl;
    std::cout << "[scpController] Total samples collected: " << collectedSamples << std::endl;
}

void scpController::collectSamples(int numberOfSamples)
{
    if (!isRunning)
    {
        std::cerr << "[scpController] ERROR: Cannot collect samples. Oscilloscope not running." << std::endl;
        return;
    }
    
    std::cout << "[scpController] Collecting " << numberOfSamples << " samples..." << std::endl;
    
    // Read samples from the device
    std::vector<uint8_t> samples = reader->readSamples(numberOfSamples);
    
    // Append to data buffer
    dataBuffer.insert(dataBuffer.end(), samples.begin(), samples.end());
    collectedSamples += samples.size();
    
    std::cout << "[scpController] Collection complete. Total samples: " << collectedSamples << std::endl;
    
    // Auto-save if output file is configured
    if (!config->getOutputFile().empty())
    {
        writer->writeData(config->getOutputFile(), dataBuffer);
    }
}

bool scpController::isActive() const
{
    return isRunning;
}

std::string scpController::getStatus() const
{
    if (isRunning)
    {
        return "RUNNING - Samples: " + std::to_string(collectedSamples);
    }
    else
    {
        return "STOPPED - Samples: " + std::to_string(collectedSamples);
    }
}

int scpController::getSamplesCollected() const
{
    return collectedSamples;
}

void scpController::readFromFile(const std::string &filename)
{
    std::cout << "[scpController] Reading data from file: " << filename << std::endl;
    dataBuffer = reader->readData(filename);
    collectedSamples = dataBuffer.size();
}

void scpController::writeToFile(const std::string &filename)
{
    if (dataBuffer.empty())
    {
        std::cerr << "[scpController] ERROR: No data to write" << std::endl;
        return;
    }
    
    std::cout << "[scpController] Writing data to file: " << filename << std::endl;
    writer->writeData(filename, dataBuffer);
}
