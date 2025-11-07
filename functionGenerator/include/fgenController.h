#ifndef FGENCONTROLLER_H
#define FGENCONTROLLER_H

#include <string>
#include <vector>
#include <cstdint>
#include "scpWriter.h"
#include "fgenConfig.h"
#include "scpWaveformGen.h"

/**
 * Class: fgenController (CLS-006)
 * Responsibility: Control function generator operations
 * Associations:
 *   - Manages scpWriter (1:1)
 *   - Configures fgenConfig (1:1)
 *   - Uses scpWaveformGen (1:1)
 */
class fgenController
{
public:
    fgenController(scpWriter *writer, fgenConfig *config, scpWaveformGen *waveGen);
    
    // Control operations
    void start();
    void stop();
    
    // Generate and output waveform
    void generateAndOutput();
    
    // Status methods
    bool isActive() const;
    std::string getStatus() const;
    
    // Configuration methods (NEW)
    void setWaveType(const std::string &typeStr);
    void setFrequency(double freq);
    void setAmplitude(double amp);
    void setOffset(double offset);
    void setNumSamples(int samples);
    void setOutputFile(const std::string &filename);
    void displayConfig() const;

private:
    scpWriter *writer;
    fgenConfig *config;
    scpWaveformGen *waveGen;
    bool isRunning;
};

#endif // FGENCONTROLLER_H