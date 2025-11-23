#ifndef SCPCONFIG_H
#define SCPCONFIG_H

#include <string>
#include <iostream>
#include <atomic>

/**
 * Class: scpConfig (CLS-005)
 * Responsibility: Store and manage oscilloscope configuration
 */
class scpConfig
{
public:
    scpConfig();
    
    // Setters
    void setSampleRate(int rate);
    void setNumberOfSamples(int n);
    void setTriggerLevel(double level);
    void setTriggerEnabled(bool enabled);
    void setInputFile(const std::string &file);
    void setOutputFile(const std::string &file);
    
    // Getters
    int getSampleRate() const;
    int getNumberOfSamples() const;
    double getTriggerLevel() const;
    bool isTriggerEnabled() const;
    std::string getInputFile() const;
    std::string getOutputFile() const;
    int getConfigId() const { return configId; }
    std::string getLabel() const { return label; }
    
    // Load/Save configuration
    bool loadFromFile(const std::string &filename);
    bool saveToFile(const std::string &filename);
    
    // Display current configuration
    void displayConfig() const;

private:
    static std::atomic<int> nextConfigId;
    
    int configId;
    std::string label;
    int sampleRate;           // Samples per second
    int numberOfSamples;      // Total samples to collect
    double triggerLevel;      // Trigger voltage level
    bool triggerEnabled;      // Enable/disable trigger
    std::string inputFile;    // Input file path
    std::string outputFile;   // Output file path
};

#endif // SCPCONFIG_H