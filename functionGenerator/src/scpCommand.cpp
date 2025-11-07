#include "scpCommand.h"
#include "scpController.h"
#include "fgenController.h"
#include <thread>
#include <chrono>

scpCommand::scpCommand(CommandType t) : type(t) {}

void scpCommand::setParameter(const std::string &key, const std::string &value)
{
    parameters[key] = value;
}

std::string scpCommand::getParameter(const std::string &key) const
{
    auto it = parameters.find(key);
    if (it != parameters.end())
    {
        return it->second;
    }
    return "";
}

bool scpCommand::hasParameter(const std::string &key) const
{
    return parameters.find(key) != parameters.end();
}

CommandType scpCommand::getType() const
{
    return type;
}

std::string scpCommand::getTypeName() const
{
    return commandTypeToString(type);
}

void scpCommand::execute(scpController *scopeCtrl, fgenController *fgenCtrl)
{
    std::cout << "\n[scpCommand] Executing: " << getTypeName() << std::endl;
    
    switch (type)
    {
        case CommandType::START_SCOPE:
            if (scopeCtrl) scopeCtrl->start();
            break;
            
        case CommandType::STOP_SCOPE:
            if (scopeCtrl) scopeCtrl->stop();
            break;
            
        case CommandType::START_FGEN:
            if (fgenCtrl) fgenCtrl->start();
            break;
            
        case CommandType::STOP_FGEN:
            if (fgenCtrl) fgenCtrl->stop();
            break;
            
        case CommandType::READ_FILE:
            if (scopeCtrl && hasParameter("filename"))
            {
                scopeCtrl->readFromFile(getParameter("filename"));
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: READ_FILE requires 'filename' parameter" << std::endl;
            }
            break;
            
        case CommandType::WRITE_FILE:
            if (scopeCtrl && hasParameter("filename"))
            {
                scopeCtrl->writeToFile(getParameter("filename"));
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: WRITE_FILE requires 'filename' parameter" << std::endl;
            }
            break;
            
        case CommandType::COLLECT_SAMPLES:
            if (scopeCtrl && hasParameter("numberOfSamples"))
            {
                int numSamples = std::stoi(getParameter("numberOfSamples"));
                scopeCtrl->collectSamples(numSamples);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: COLLECT_SAMPLES requires 'numberOfSamples' parameter" << std::endl;
            }
            break;
            
        case CommandType::WAIT:
            if (hasParameter("seconds"))
            {
                int seconds = std::stoi(getParameter("seconds"));
                std::cout << "[scpCommand] Waiting for " << seconds << " seconds..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                std::cout << "[scpCommand] Wait complete" << std::endl;
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: WAIT requires 'seconds' parameter" << std::endl;
            }
            break;
        
        // FUNCTION GENERATOR COMMANDS
        case CommandType::GENERATE_WAVEFORM:
            if (fgenCtrl)
            {
                fgenCtrl->generateAndOutput();
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: Function generator controller not available" << std::endl;
            }
            break;
            
        case CommandType::SET_WAVE_TYPE:
            if (fgenCtrl && hasParameter("type"))
            {
                std::string typeStr = getParameter("type");
                fgenCtrl->setWaveType(typeStr);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_WAVE_TYPE requires 'type' parameter (SINE/SQUARE/TRIANGLE/SAWTOOTH)" << std::endl;
            }
            break;
            
        case CommandType::SET_FREQUENCY:
            if (fgenCtrl && hasParameter("value"))
            {
                double freq = std::stod(getParameter("value"));
                fgenCtrl->setFrequency(freq);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_FREQUENCY requires 'value' parameter" << std::endl;
            }
            break;
            
        case CommandType::SET_AMPLITUDE:
            if (fgenCtrl && hasParameter("value"))
            {
                double amp = std::stod(getParameter("value"));
                fgenCtrl->setAmplitude(amp);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_AMPLITUDE requires 'value' parameter" << std::endl;
            }
            break;
            
        case CommandType::SET_OFFSET:
            if (fgenCtrl && hasParameter("value"))
            {
                double offset = std::stod(getParameter("value"));
                fgenCtrl->setOffset(offset);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_OFFSET requires 'value' parameter" << std::endl;
            }
            break;
            
        case CommandType::SET_NUM_SAMPLES:
            if (fgenCtrl && hasParameter("value"))
            {
                int samples = std::stoi(getParameter("value"));
                fgenCtrl->setNumSamples(samples);
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_NUM_SAMPLES requires 'value' parameter" << std::endl;
            }
            break;
            
        case CommandType::SET_OUTPUT_FILE:
            if (fgenCtrl && hasParameter("filename"))
            {
                fgenCtrl->setOutputFile(getParameter("filename"));
            }
            else
            {
                std::cerr << "[scpCommand] ERROR: SET_OUTPUT_FILE requires 'filename' parameter" << std::endl;
            }
            break;
            
        default:
            std::cerr << "[scpCommand] ERROR: Unknown command type" << std::endl;
            break;
    }
}

bool scpCommand::validate() const
{
    switch (type)
    {
        case CommandType::READ_FILE:
        case CommandType::WRITE_FILE:
            return hasParameter("filename");
            
        case CommandType::COLLECT_SAMPLES:
            return hasParameter("numberOfSamples");
            
        case CommandType::WAIT:
            return hasParameter("seconds");
            
        case CommandType::SET_WAVE_TYPE:
            return hasParameter("type");
            
        case CommandType::SET_FREQUENCY:
        case CommandType::SET_AMPLITUDE:
        case CommandType::SET_OFFSET:
        case CommandType::SET_NUM_SAMPLES:
            return hasParameter("value");
            
        case CommandType::SET_OUTPUT_FILE:
            return hasParameter("filename");
            
        case CommandType::START_SCOPE:
        case CommandType::STOP_SCOPE:
        case CommandType::START_FGEN:
        case CommandType::STOP_FGEN:
        case CommandType::GENERATE_WAVEFORM:
            return true; // No parameters required
            
        default:
            return false;
    }
}

CommandType scpCommand::stringToCommandType(const std::string &str)
{
    if (str == "START_SCOPE" || str == "start_scope") return CommandType::START_SCOPE;
    if (str == "STOP_SCOPE" || str == "stop_scope") return CommandType::STOP_SCOPE;
    if (str == "START_FGEN" || str == "start_fgen") return CommandType::START_FGEN;
    if (str == "STOP_FGEN" || str == "stop_fgen") return CommandType::STOP_FGEN;
    if (str == "READ_FILE" || str == "read_file") return CommandType::READ_FILE;
    if (str == "WRITE_FILE" || str == "write_file") return CommandType::WRITE_FILE;
    if (str == "COLLECT_SAMPLES" || str == "collect_samples") return CommandType::COLLECT_SAMPLES;
    if (str == "WAIT" || str == "wait") return CommandType::WAIT;
    
    // FUNCTION GENERATOR COMMANDS
    if (str == "GENERATE_WAVEFORM" || str == "generate_waveform") return CommandType::GENERATE_WAVEFORM;
    if (str == "SET_WAVE_TYPE" || str == "set_wave_type") return CommandType::SET_WAVE_TYPE;
    if (str == "SET_FREQUENCY" || str == "set_frequency") return CommandType::SET_FREQUENCY;
    if (str == "SET_AMPLITUDE" || str == "set_amplitude") return CommandType::SET_AMPLITUDE;
    if (str == "SET_OFFSET" || str == "set_offset") return CommandType::SET_OFFSET;
    if (str == "SET_NUM_SAMPLES" || str == "set_num_samples") return CommandType::SET_NUM_SAMPLES;
    if (str == "SET_OUTPUT_FILE" || str == "set_output_file") return CommandType::SET_OUTPUT_FILE;
    
    return CommandType::UNKNOWN;
}

std::string scpCommand::commandTypeToString(CommandType type)
{
    switch (type)
    {
        case CommandType::START_SCOPE: return "START_SCOPE";
        case CommandType::STOP_SCOPE: return "STOP_SCOPE";
        case CommandType::START_FGEN: return "START_FGEN";
        case CommandType::STOP_FGEN: return "STOP_FGEN";
        case CommandType::READ_FILE: return "READ_FILE";
        case CommandType::WRITE_FILE: return "WRITE_FILE";
        case CommandType::COLLECT_SAMPLES: return "COLLECT_SAMPLES";
        case CommandType::WAIT: return "WAIT";
        
        // FUNCTION GENERATOR COMMANDS
        case CommandType::GENERATE_WAVEFORM: return "GENERATE_WAVEFORM";
        case CommandType::SET_WAVE_TYPE: return "SET_WAVE_TYPE";
        case CommandType::SET_FREQUENCY: return "SET_FREQUENCY";
        case CommandType::SET_AMPLITUDE: return "SET_AMPLITUDE";
        case CommandType::SET_OFFSET: return "SET_OFFSET";
        case CommandType::SET_NUM_SAMPLES: return "SET_NUM_SAMPLES";
        case CommandType::SET_OUTPUT_FILE: return "SET_OUTPUT_FILE";
        
        default: return "UNKNOWN";
    }
}