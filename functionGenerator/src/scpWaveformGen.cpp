#include "scpWaveformGen.h"
#include <iostream>
#include <algorithm>

scpWaveformGen::scpWaveformGen(fgenConfig *cfg) : config(cfg)
{
    std::cout << "[scpWaveformGen] Waveform generator initialized" << std::endl;
}

uint8_t scpWaveformGen::voltageToUint8(double voltage)
{
    // Map voltage to 0-255 range
    // Assuming voltage range is -5V to +5V mapped to 0-255
    double normalized = (voltage + 5.0) / 10.0; // Normalize to 0-1
    normalized = std::max(0.0, std::min(1.0, normalized)); // Clamp
    return static_cast<uint8_t>(normalized * 255);
}

std::vector<uint8_t> scpWaveformGen::generateSine()
{
    int numSamples = config->getNumSamples();
    double amplitude = config->getAmplitude();
    double offset = config->getOffset();
    
    std::vector<uint8_t> waveform(numSamples);
    
    for (int i = 0; i < numSamples; ++i)
    {
        double angle = 2.0 * PI * i / numSamples;
        double voltage = amplitude * std::sin(angle) + offset;
        waveform[i] = voltageToUint8(voltage);
    }
    
    std::cout << "[scpWaveformGen] Generated SINE waveform with " << numSamples << " samples" << std::endl;
    return waveform;
}

std::vector<uint8_t> scpWaveformGen::generateSquare()
{
    int numSamples = config->getNumSamples();
    double amplitude = config->getAmplitude();
    double offset = config->getOffset();
    
    std::vector<uint8_t> waveform(numSamples);
    
    for (int i = 0; i < numSamples; ++i)
    {
        double voltage = (i < numSamples / 2) ? amplitude + offset : -amplitude + offset;
        waveform[i] = voltageToUint8(voltage);
    }
    
    std::cout << "[scpWaveformGen] Generated SQUARE waveform with " << numSamples << " samples" << std::endl;
    return waveform;
}

std::vector<uint8_t> scpWaveformGen::generateTriangle()
{
    int numSamples = config->getNumSamples();
    double amplitude = config->getAmplitude();
    double offset = config->getOffset();
    
    std::vector<uint8_t> waveform(numSamples);
    
    for (int i = 0; i < numSamples; ++i)
    {
        double voltage;
        if (i < numSamples / 2)
        {
            // Rising edge
            voltage = (4.0 * amplitude * i / numSamples) - amplitude + offset;
        }
        else
        {
            // Falling edge
            voltage = (-4.0 * amplitude * (i - numSamples / 2) / numSamples) + amplitude + offset;
        }
        waveform[i] = voltageToUint8(voltage);
    }
    
    std::cout << "[scpWaveformGen] Generated TRIANGLE waveform with " << numSamples << " samples" << std::endl;
    return waveform;
}

std::vector<uint8_t> scpWaveformGen::generateSawtooth()
{
    int numSamples = config->getNumSamples();
    double amplitude = config->getAmplitude();
    double offset = config->getOffset();
    
    std::vector<uint8_t> waveform(numSamples);
    
    for (int i = 0; i < numSamples; ++i)
    {
        double voltage = (2.0 * amplitude * i / numSamples) - amplitude + offset;
        waveform[i] = voltageToUint8(voltage);
    }
    
    std::cout << "[scpWaveformGen] Generated SAWTOOTH waveform with " << numSamples << " samples" << std::endl;
    return waveform;
}

std::vector<uint8_t> scpWaveformGen::generateWaveform()
{
    WaveType type = config->getWaveType();
    
    switch (type)
    {
        case WaveType::SINE:
            return generateSine();
        case WaveType::SQUARE:
            return generateSquare();
        case WaveType::TRIANGLE:
            return generateTriangle();
        case WaveType::SAWTOOTH:
            return generateSawtooth();
        default:
            std::cerr << "[scpWaveformGen] ERROR: Unknown waveform type" << std::endl;
            return {};
    }
}
