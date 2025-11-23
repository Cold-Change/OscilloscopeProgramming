#include "scpController.h"
#include "scpTimer.h"
#include "scpDataCollector.h"
#include <iostream>
#include <chrono>

scpController::scpController(scpReader *r, scpWriter *w, scpConfig *c)
    : reader(r), writer(w), config(c), isRunning(false), collectedSamples(0)
{
    std::cout << "[scpController] Oscilloscope controller initialized" << std::endl;
}

scpController::~scpController() {
    if (dataCollector) {
        dataCollector->stop();
        dataCollector->join();
    }
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
    
    // Stop any ongoing collection
    if (dataCollector) {
        dataCollector->stop();
        dataCollector->join();
        dataCollector.reset();
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

void scpController::collectSamplesThreaded(int durationSeconds)
{
    if (!isRunning)
    {
        std::cerr << "[scpController] ERROR: Cannot collect samples. Oscilloscope not running." << std::endl;
        std::cerr << "[scpController] Please use START_SCOPE first." << std::endl;
        return;
    }
    
    std::cout << "\n========== Multi-threaded Data Collection ==========" << std::endl;
    std::cout << "[scpController] Duration: " << durationSeconds << " seconds" << std::endl;
    std::cout << "[scpController] Sample Rate: " << config->getSampleRate() << " Hz" << std::endl;
    std::cout << "====================================================\n" << std::endl;
    
    // Create timer thread
    scp::Timer timer(std::chrono::seconds(durationSeconds), coutMutex);
    
    // Create data collector thread
    dataCollector = std::make_unique<scp::DataCollector>(reader, config, coutMutex);
    
    // Start both threads
    timer.start();
    dataCollector->start(std::chrono::seconds(durationSeconds));
    
    // Wait for both threads to complete
    timer.join();
    dataCollector->join();
    
    // Retrieve collected data
    auto result = dataCollector->getResult();
    
    // Update buffer and counters
    dataBuffer.insert(dataBuffer.end(), result.data.begin(), result.data.end());
    collectedSamples += result.samplesCollected;
    
    std::cout << "\n========== Collection Summary ==========" << std::endl;
    std::cout << "[scpController] Samples collected: " << result.samplesCollected << std::endl;
    std::cout << "[scpController] Total time: " << result.totalTime.count() << " ms" << std::endl;
    std::cout << "[scpController] Average rate: " 
              << (result.totalTime.count() > 0 ? 
                  (result.samplesCollected * 1000.0 / result.totalTime.count()) : 0.0)
              << " Hz" << std::endl;
    std::cout << "[scpController] Total samples in buffer: " << collectedSamples << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Auto-save if output file is configured
    if (!config->getOutputFile().empty())
    {
        writer->writeData(config->getOutputFile(), dataBuffer);
        std::cout << "[scpController] Data written to: " << config->getOutputFile() << std::endl;
    }
    
    // Clean up collector
    dataCollector.reset();
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