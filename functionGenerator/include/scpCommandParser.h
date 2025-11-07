#ifndef SCPCOMMANDPARSER_H
#define SCPCOMMANDPARSER_H

#include <vector>
#include <string>
#include "scpCommand.h"
#include "scpController.h"
#include "fgenController.h"

class scpCommandParser
{
public:
    scpCommandParser(scpController *scopeCtrl, fgenController *fgenCtrl);
    
    // Parse commands from different sources
    bool parseCommandLine(int argc, char *argv[]);
    bool parseFile(const std::string &filename);
    
    // Execute all parsed commands
    void executeCommands();
    
    // Command management
    void addCommand(const scpCommand &cmd);
    std::vector<scpCommand> getCommands() const;
    void clearCommands();
    
    // Display commands
    void displayCommands() const;

private:
    std::vector<scpCommand> commands;
    scpController *scopeController;
    fgenController *fgenController_;
    
    // Helper functions
    scpCommand parseCommandString(const std::string &cmdStr);
    void parseParameters(scpCommand &cmd, const std::string &paramStr);
};

#endif // SCPCOMMANDPARSER_H
