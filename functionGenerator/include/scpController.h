#ifndef SCPCONTROLLER_H
#define SCPCONTROLLER_H

#include <string>
#include <vector>
#include <cstdint>
#include <mutex>
#include <memory>
#include <map>
#include <algorithm>
#include "scpReader.h"
#include "scpWriter.h"
#include "scpConfig.h"

// Forward declaration
namespace scp {
    class DataCollector;
}

class scpController
{
public:
    scpController(scpReader *reader, scpWriter *writer, scpConfig *config);
    ~scpController();
    
    // Control operations
    void start();
    void stop();
    
    // Data collection
    void collectSamples(int numberOfSamples);
    void collectSamplesThreaded(int durationSeconds);
    
    // Status methods
    bool isActive() const;
    std::string getStatus() const;
    int getSamplesCollected() const;
    
    // File operations
    void readFromFile(const std::string &filename);
    void writeToFile(const std::string &filename = "");
    
    // Frequency control
    void setSamplingFrequency(double freqHz);
    void setBaudRate(int baud);
    
    // NEW: Data visualization
    void displayDataPreview(int numSamples = 64) const;

private:
    scpReader *reader;
    scpWriter *writer;
    scpConfig *config;
    bool isRunning;
    int collectedSamples;
    std::vector<uint8_t> dataBuffer;
    
    std::mutex coutMutex;
    std::unique_ptr<scp::DataCollector> dataCollector;
    
    static const std::string DEFAULT_OUTPUT_FILE;
};

#endif