#include "scpController.h"
#include "scpTimer.h"
#include "scpDataCollector.h"
#include <iostream>
#include <iomanip>
#include <chrono>

// Define default output file
const std::string scpController::DEFAULT_OUTPUT_FILE = "output.dat";

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

// NEW: Display data preview
void scpController::displayDataPreview(int numSamples) const
{
    if (dataBuffer.empty())
    {
        std::cout << "[scpController] No data to display" << std::endl;
        return;
    }
    
    int samplesToShow = std::min(numSamples, static_cast<int>(dataBuffer.size()));
    
    std::cout << "\n========== Data Preview (First " << samplesToShow << " samples) ==========" << std::endl;
    std::cout << "Offset    Hex Values                                      ASCII" << std::endl;
    std::cout << "--------  ------------------------------------------------  ----------------" << std::endl;
    
    for (int i = 0; i < samplesToShow; i += 16)
    {
        // Offset
        std::cout << std::setw(8) << std::setfill('0') << std::hex << i << "  ";
        
        // Hex values
        int lineEnd = std::min(i + 16, samplesToShow);
        for (int j = i; j < lineEnd; j++)
        {
            std::cout << std::setw(2) << std::setfill('0') << std::hex 
                      << static_cast<int>(dataBuffer[j]) << " ";
        }
        
        // Padding for incomplete lines
        for (int j = lineEnd; j < i + 16; j++)
        {
            std::cout << "   ";
        }
        
        std::cout << " ";
        
        // ASCII representation
        for (int j = i; j < lineEnd; j++)
        {
            unsigned char c = dataBuffer[j];
            std::cout << (c >= 32 && c <= 126 ? static_cast<char>(c) : '.');
        }
        
        std::cout << std::dec << std::endl;
    }
    
    std::cout << "========================================" << std::endl;
    
    // Statistics
    std::map<uint8_t, int> histogram;
    for (auto byte : dataBuffer)
    {
        histogram[byte]++;
    }
    
    std::cout << "\n=== Data Statistics ===" << std::endl;
    std::cout << "Total samples: " << dataBuffer.size() << std::endl;
    std::cout << "Unique values: " << histogram.size() << std::endl;
    
    // Show top 5 most common values
    std::vector<std::pair<uint8_t, int>> sortedHist(histogram.begin(), histogram.end());
    std::sort(sortedHist.begin(), sortedHist.end(), 
              [](const auto &a, const auto &b) { return a.second > b.second; });
    
    std::cout << "\nTop 5 most common values:" << std::endl;
    for (int i = 0; i < std::min(5, static_cast<int>(sortedHist.size())); i++)
    {
        std::cout << "  0x" << std::setw(2) << std::setfill('0') << std::hex 
                  << static_cast<int>(sortedHist[i].first) << " (" 
                  << std::dec << static_cast<int>(sortedHist[i].first) << "): "
                  << sortedHist[i].second << " times ("
                  << std::fixed << std::setprecision(1)
                  << (100.0 * sortedHist[i].second / dataBuffer.size()) << "%)" << std::endl;
    }
    std::cout << "=======================\n" << std::endl;
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
    
    // Show preview of collected data
    displayDataPreview(64);  // Show first 64 bytes
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
    
    // Show preview of collected data
    displayDataPreview(64);  // Show first 64 bytes
    
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
    
    // Show preview
    displayDataPreview(64);
}

void scpController::writeToFile(const std::string &filename)
{
    if (dataBuffer.empty())
    {
        std::cerr << "[scpController] ERROR: No data to write" << std::endl;
        return;
    }
    
    // Use provided filename or default
    std::string outputFile = filename.empty() ? 
                             (config->getOutputFile().empty() ? DEFAULT_OUTPUT_FILE : config->getOutputFile()) : 
                             filename;
    
    std::cout << "[scpController] Writing " << dataBuffer.size() << " bytes to: " << outputFile << std::endl;
    writer->writeData(outputFile, dataBuffer);
    
    std::cout << "[scpController] ✓ File written successfully: " << outputFile << std::endl;
}

// Frequency control methods
void scpController::setSamplingFrequency(double freqHz)
{
    if (!isRunning)
    {
        std::cerr << "[scpController] WARNING: Oscilloscope not running. Configuration will apply on next start." << std::endl;
    }
    
    std::cout << "[scpController] Setting sampling frequency to " << freqHz << " Hz" << std::endl;
    
    config->setSamplingFrequency(freqHz);
    int baudRate = config->getBaudRate();
    reader->setBaudRate(baudRate);
    
    std::cout << "[scpController] Baud rate configured: " << baudRate << " baud" << std::endl;
    std::cout << "[scpController] Effective frequency: " << config->getEffectiveFrequency() << " Hz" << std::endl;
}

void scpController::setBaudRate(int baud)
{
    if (!isRunning)
    {
        std::cerr << "[scpController] WARNING: Oscilloscope not running. Configuration will apply on next start." << std::endl;
    }
    
    std::cout << "[scpController] Setting baud rate to " << baud << " baud" << std::endl;
    
    config->setBaudRate(baud);
    reader->setBaudRate(baud);
    
    std::cout << "[scpController] Effective sampling frequency: " << config->getEffectiveFrequency() << " Hz" << std::endl;
}