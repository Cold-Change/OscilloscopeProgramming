#ifndef SCPWAVEFORMGEN_H
#define SCPWAVEFORMGEN_H

#include <vector>
#include <cstdint>
#include <cmath>
#include "fgenConfig.h"

/**
 * Class: scpWaveformGen (CLS-008)
 * Responsibility: Generate various waveform types
 * Association: Uses fgenConfig (1:1)
 */
class scpWaveformGen
{
public:
    explicit scpWaveformGen(fgenConfig *config);
    
    // Generate specific waveforms
    std::vector<uint8_t> generateSine();
    std::vector<uint8_t> generateSquare();
    std::vector<uint8_t> generateTriangle();
    std::vector<uint8_t> generateSawtooth();
    
    // Generate based on current configuration
    std::vector<uint8_t> generateWaveform();

private:
    fgenConfig *config;
    
    // Helper function to convert voltage to uint8_t (0-255)
    uint8_t voltageToUint8(double voltage);
    
    // Constants
    static constexpr double PI = 3.14159265358979323846;
};

#endif // SCPWAVEFORMGEN_H
