#ifndef SCPCONFIG_H
#define SCPCONFIG_H

#include <string>
#include <iostream>

/**
 * Class: scpConfig (CLS-005)
 * Responsibility: Store and manage oscilloscope configuration
 * Association: Used by scpController (1:1)
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
    
    // Load/Save configuration
    bool loadFromFile(const std::string &filename);
    bool saveToFile(const std::string &filename);
    
    // Display current configuration
    void displayConfig() const;

private:
    int sampleRate;           // Samples per second
    int numberOfSamples;      // Total samples to collect
    double triggerLevel;      // Trigger voltage level
    bool triggerEnabled;      // Enable/disable trigger
    std::string inputFile;    // Input file path
    std::string outputFile;   // Output file path
};

#endif // SCPCONFIG_H
