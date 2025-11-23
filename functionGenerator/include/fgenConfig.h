#ifndef FGENCONFIG_H
#define FGENCONFIG_H

#include <string>
#include <iostream>
#include <atomic>

/**
 * Enum: WaveType
 * Different waveform types supported by function generator
 */
enum class WaveType
{
    SINE = 0,
    SQUARE = 1,
    TRIANGLE = 2,
    SAWTOOTH = 3
};

/**
 * Class: fgenConfig (CLS-007)
 * Responsibility: Store function generator configuration
 */
class fgenConfig
{
public:
    fgenConfig();
    
    // Setters
    void setFrequency(double freq);
    void setAmplitude(double amp);
    void setOffset(double off);
    void setWaveType(WaveType type);
    void setOutputFile(const std::string &file);
    void setNumSamples(int samples);
    
    // Getters
    double getFrequency() const;
    double getAmplitude() const;
    double getOffset() const;
    WaveType getWaveType() const;
    std::string getOutputFile() const;
    int getNumSamples() const;
    int getConfigId() const { return configId; }
    std::string getLabel() const { return label; }
    
    // Utility
    std::string waveTypeToString() const;
    std::string waveTypeToStringWithId(WaveType type) const;
    static WaveType stringToWaveType(const std::string &str);
    static std::string getWaveTypeLabel(WaveType type);
    static int getWaveTypeId(WaveType type);
    
    // Load/Save configuration
    bool loadFromFile(const std::string &filename);
    bool saveToFile(const std::string &filename);
    
    // Display current configuration
    void displayConfig() const;

private:
    static std::atomic<int> nextConfigId;
    
    int configId;
    std::string label;
    double frequency;         // Frequency in Hz
    double amplitude;         // Amplitude in volts
    double offset;            // DC offset in volts
    WaveType waveType;        // Type of waveform
    std::string outputFile;   // Output file path
    int numSamples;           // Number of samples per waveform
};

#endif // FGENCONFIG_H