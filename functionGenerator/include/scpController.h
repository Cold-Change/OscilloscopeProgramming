#ifndef SCPCONTROLLER_H
#define SCPCONTROLLER_H

#include <string>
#include <vector>
#include <cstdint>
#include "scpReader.h"
#include "scpWriter.h"
#include "scpConfig.h"

/**
 * Class: scpController (CLS-004)
 * Responsibility: Orchestrate oscilloscope operations
 * Associations: 
 *   - Manages scpReader (1:1)
 *   - Manages scpWriter (1:1)
 *   - Configures scpConfig (1:1)
 */
class scpController
{
public:
    scpController(scpReader *reader, scpWriter *writer, scpConfig *config);
    
    // Control operations
    void start();
    void stop();
    
    // Data collection
    void collectSamples(int numberOfSamples);
    
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
};

#endif // SCPCONTROLLER_H
