#include "scpCommandParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

scpCommandParser::scpCommandParser(scpController *scopeCtrl, fgenController *fgenCtrl)
    : scopeController(scopeCtrl), fgenController_(fgenCtrl)
{
    std::cout << "[scpCommandParser] Command parser initialized" << std::endl;
}

bool scpCommandParser::parseCommandLine(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "[scpCommandParser] ERROR: No commands provided" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <command1> [param1=value1] [command2] ..." << std::endl;
        std::cerr << "\nCommand format: COMMAND_TYPE param1=value1 param2=value2 ..." << std::endl;
        std::cerr << "\nExamples:" << std::endl;
        std::cerr << "  START_SCOPE" << std::endl;
        std::cerr << "  COLLECT_SAMPLES numberOfSamples=10000" << std::endl;
        std::cerr << "  WRITE_FILE filename=output.dat" << std::endl;
        std::cerr << "  WAIT seconds=5" << std::endl;
        std::cerr << "  STOP_SCOPE" << std::endl;
        return false;
    }
    
    // Parse command line arguments - group commands with their parameters
    int i = 1;
    while (i < argc)
    {
        std::string cmdStr = argv[i];
        CommandType cmdType = scpCommand::stringToCommandType(cmdStr);
        
        if (cmdType != CommandType::UNKNOWN)
        {
            // This is a valid command
            scpCommand cmd(cmdType);
            
            // Look ahead for parameters (arguments with '=' sign)
            i++;
            while (i < argc && std::string(argv[i]).find('=') != std::string::npos)
            {
                parseParameters(cmd, argv[i]);
                i++;
            }
            
            commands.push_back(cmd);
        }
        else
        {
            std::cerr << "[scpCommandParser] WARNING: Unknown command ignored: " << cmdStr << std::endl;
            i++;
        }
    }
    
    std::cout << "[scpCommandParser] Parsed " << commands.size() << " commands from command line" << std::endl;
    return !commands.empty();
}

bool scpCommandParser::parseFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "[scpCommandParser] ERROR: Cannot open command file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int lineNum = 0;
    
    while (std::getline(file, line))
    {
        lineNum++;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;
        
        scpCommand cmd = parseCommandString(line);
        
        if (cmd.getType() != CommandType::UNKNOWN)
        {
            commands.push_back(cmd);
        }
        else
        {
            std::cerr << "[scpCommandParser] WARNING: Line " << lineNum 
                     << " - Unknown command ignored: " << line << std::endl;
        }
    }
    
    std::cout << "[scpCommandParser] Parsed " << commands.size() 
             << " commands from file " << filename << std::endl;
    return !commands.empty();
}

scpCommand scpCommandParser::parseCommandString(const std::string &cmdStr)
{
    std::istringstream iss(cmdStr);
    std::string cmdTypeStr;
    iss >> cmdTypeStr;
    
    CommandType cmdType = scpCommand::stringToCommandType(cmdTypeStr);
    scpCommand cmd(cmdType);
    
    // Parse remaining parameters
    std::string paramStr;
    while (iss >> paramStr)
    {
        parseParameters(cmd, paramStr);
    }
    
    return cmd;
}

void scpCommandParser::parseParameters(scpCommand &cmd, const std::string &paramStr)
{
    size_t equalPos = paramStr.find('=');
    if (equalPos != std::string::npos)
    {
        std::string key = paramStr.substr(0, equalPos);
        std::string value = paramStr.substr(equalPos + 1);
        cmd.setParameter(key, value);
        std::cout << "[scpCommandParser] Parameter added: " << key << "=" << value << std::endl;
    }
    else
    {
        std::cerr << "[scpCommandParser] WARNING: Invalid parameter format (expected key=value): " 
                 << paramStr << std::endl;
    }
}

void scpCommandParser::executeCommands()
{
    if (commands.empty())
    {
        std::cout << "[scpCommandParser] No commands to execute" << std::endl;
        return;
    }
    
    std::cout << "\n[scpCommandParser] ========== Executing " << commands.size() 
             << " commands ==========" << std::endl;
    
    for (size_t i = 0; i < commands.size(); ++i)
    {
        std::cout << "\n--- Command " << (i + 1) << "/" << commands.size() << " ---" << std::endl;
        
        if (commands[i].validate())
        {
            commands[i].execute(scopeController, fgenController_);
        }
        else
        {
            std::cerr << "[scpCommandParser] ERROR: Command validation failed for " 
                     << commands[i].getTypeName() << std::endl;
            std::cerr << "[scpCommandParser] Missing required parameters" << std::endl;
        }
    }
    
    std::cout << "\n[scpCommandParser] ========== Execution complete ==========" << std::endl;
}

void scpCommandParser::addCommand(const scpCommand &cmd)
{
    commands.push_back(cmd);
}

std::vector<scpCommand> scpCommandParser::getCommands() const
{
    return commands;
}

void scpCommandParser::clearCommands()
{
    commands.clear();
    std::cout << "[scpCommandParser] Commands cleared" << std::endl;
}

void scpCommandParser::displayCommands() const
{
    std::cout << "\n=== Command Queue (" << commands.size() << " commands) ===" << std::endl;
    
    for (size_t i = 0; i < commands.size(); ++i)
    {
        std::cout << i + 1 << ". " << commands[i].getTypeName();
        
        // Display parameters if any
        if (commands[i].hasParameter("numberOfSamples"))
            std::cout << " (numberOfSamples=" << commands[i].getParameter("numberOfSamples") << ")";
        if (commands[i].hasParameter("filename"))
            std::cout << " (filename=" << commands[i].getParameter("filename") << ")";
        if (commands[i].hasParameter("seconds"))
            std::cout << " (seconds=" << commands[i].getParameter("seconds") << ")";
        
        std::cout << std::endl;
    }
    
    std::cout << "========================================\n" << std::endl;
}