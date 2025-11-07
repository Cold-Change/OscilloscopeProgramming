#ifndef SCPCOMMAND_H
#define SCPCOMMAND_H

#include <string>
#include <map>
#include <iostream>

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
    START_SCOPE,
    STOP_SCOPE,
    COLLECT_SAMPLES,
    READ_FILE,
    WRITE_FILE,
    
    // Function Generator commands
    START_FGEN,
    STOP_FGEN,
    GENERATE_WAVEFORM,
    SET_WAVE_TYPE,
    SET_FREQUENCY,
    SET_AMPLITUDE,
    SET_OFFSET,
    SET_NUM_SAMPLES,
    SET_OUTPUT_FILE,
    
    // Utility commands
    WAIT,
    
    UNKNOWN
};

/**
 * Class: scpCommand (CLS-011)
 * Responsibility: Represent and execute single command
 * Association: Parsed by scpCommandParser (1:0..*)
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
    
    // Execute the command
    void execute(scpController *scopeCtrl, fgenController *fgenCtrl);
    
    // Validate command parameters
    bool validate() const;
    
    // Utility
    static CommandType stringToCommandType(const std::string &str);
    static std::string commandTypeToString(CommandType type);

private:
    CommandType type;
    std::map<std::string, std::string> parameters;
};

#endif // SCPCOMMAND_H