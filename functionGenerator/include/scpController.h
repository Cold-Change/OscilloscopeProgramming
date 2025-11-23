#ifndef SCPCONTROLLER_H
#define SCPCONTROLLER_H

#include <string>
#include <vector>
#include <cstdint>
#include <mutex>
#include <memory>
#include "scpReader.h"
#include "scpWriter.h"
#include "scpConfig.h"

// Forward declaration to avoid circular dependency
namespace scp {
    class DataCollector;
}

/**
 * Class: scpController (CLS-004)
 * Responsibility: Orchestrate oscilloscope operations with multi-threading support
 * Associations: 
 *   - Manages scpReader (1:1)
 *   - Manages scpWriter (1:1)
 *   - Configures scpConfig (1:1)
 *   - Uses scpDataCollector (1:0..1) for threaded collection
 */
class scpController
{
public:
    scpController(scpReader *reader, scpWriter *writer, scpConfig *config);
    ~scpController();
    
    // Control operations
    void start();
    void stop();
    
    // Data collection - legacy single-threaded
    void collectSamples(int numberOfSamples);
    
    // NEW: Multi-threaded data collection with wait time
    void collectSamplesThreaded(int durationSeconds);
    
    // Status methods
    bool isActive() const;
    std::string getStatus() const;
    int getSamplesCollected() const;
    
    // File operations
    void readFromFile(const std::string &filename);
    void writeToFile(const std::string &filename);

private:
    scpReader *reader;
    scpWriter *writer;
    scpConfig *config;
    bool isRunning;
    int collectedSamples;
    std::vector<uint8_t> dataBuffer;
    
    // Thread synchronization
    std::mutex coutMutex;
    std::unique_ptr<scp::DataCollector> dataCollector;
};

#endif // SCPCONTROLLER_H