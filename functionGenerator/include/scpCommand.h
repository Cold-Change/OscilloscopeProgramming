#ifndef SCPCOMMAND_H
#define SCPCOMMAND_H

#include <string>
#include <map>
#include <iostream>
#include <atomic>

// Forward declarations
class scpController;
class fgenController;

/**
 * Enum: CommandType
 * Different command types supported by the system
 */
enum class CommandType
{
    // Oscilloscope commands
    START_SCOPE = 0,
    STOP_SCOPE = 1,
    COLLECT_SAMPLES = 2,
    COLLECT_SAMPLES_THREADED = 3,
    READ_FILE = 4,
    WRITE_FILE = 5,
    
    // Function Generator commands
    START_FGEN = 10,
    STOP_FGEN = 11,
    GENERATE_WAVEFORM = 12,
    SET_WAVE_TYPE = 13,
    SET_FREQUENCY = 14,
    SET_AMPLITUDE = 15,
    SET_OFFSET = 16,
    SET_NUM_SAMPLES = 17,
    SET_OUTPUT_FILE = 18,
    
    // Utility commands
    WAIT = 20,
    
    UNKNOWN = 99
};

/**
 * Class: scpCommand (CLS-011)
 * Responsibility: Represent and execute single command
 */
class scpCommand
{
public:
    explicit scpCommand(CommandType type);
    
    // Parameter management
    void setParameter(const std::string &key, const std::string &value);
    std::string getParameter(const std::string &key) const;
    bool hasParameter(const std::string &key) const;
    
    // Getters
    CommandType getType() const;
    std::string getTypeName() const;
    std::string getTypeNameWithId() const;
    int getCommandId() const { return commandId; }
    std::string getLabel() const { return label; }
    
    // Execute the command
    void execute(scpController *scopeCtrl, fgenController *fgenCtrl);
    
    // Validate command parameters
    bool validate() const;
    
    // Utility
    static CommandType stringToCommandType(const std::string &str);
    static std::string commandTypeToString(CommandType type);
    static int getCommandTypeId(CommandType type);

private:
    static std::atomic<int> nextCommandId;
    
    int commandId;
    std::string label;
    CommandType type;
    std::map<std::string, std::string> parameters;
};

#endif // SCPCOMMAND_H