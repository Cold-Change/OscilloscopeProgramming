#include <iostream>
#include <string>
#include "FtdiDevice.h"
#include "scpReader.h"
#include "scpWriter.h"
#include "scpConfig.h"
#include "scpController.h"
#include "fgenConfig.h"
#include "scpWaveformGen.h"
#include "fgenController.h"
#include "scpCommand.h"
#include "scpCommandParser.h"

void printUsage(const char *progName)
{
    std::cout << "\n========== Oscilloscope & Function Generator System ==========" << std::endl;
    std::cout << "Usage: " << progName << " [OPTIONS] <commands...>" << std::endl;
    std::cout << "\nOPTIONS:" << std::endl;
    std::cout << "  -f <file>    : Parse commands from file" << std::endl;
    std::cout << "  -h, --help   : Display this help message" << std::endl;
    std::cout << "\nCOMMANDS (Command Line):" << std::endl;
    std::cout << "  START_SCOPE                                  : Start oscilloscope" << std::endl;
    std::cout << "  STOP_SCOPE                                   : Stop oscilloscope" << std::endl;
    std::cout << "  START_FGEN                                   : Start function generator" << std::endl;
    std::cout << "  STOP_FGEN                                    : Stop function generator" << std::endl;
    std::cout << "  COLLECT_SAMPLES numberOfSamples=N            : Collect N samples (single-threaded)" << std::endl;
    std::cout << "  COLLECT_SAMPLES_THREADED waitSeconds=N       : Collect samples for N seconds (multi-threaded)" << std::endl;
    std::cout << "  READ_FILE filename=<path>                    : Read data from file" << std::endl;
    std::cout << "  WRITE_FILE filename=<path>                   : Write data to file" << std::endl;
    std::cout << "  WAIT seconds=N                               : Wait for N seconds" << std::endl;
    std::cout << "\nEXAMPLES:" << std::endl;
    std::cout << "  " << progName << " START_SCOPE COLLECT_SAMPLES numberOfSamples=10000 STOP_SCOPE" << std::endl;
    std::cout << "  " << progName << " START_SCOPE COLLECT_SAMPLES_THREADED waitSeconds=5 WRITE_FILE filename=output.dat STOP_SCOPE" << std::endl;
    std::cout << "  " << progName << " -f commands.txt" << std::endl;
    std::cout << "\nCOMMAND FILE FORMAT:" << std::endl;
    std::cout << "  # This is a comment" << std::endl;
    std::cout << "  START_SCOPE" << std::endl;
    std::cout << "  COLLECT_SAMPLES_THREADED waitSeconds=5" << std::endl;
    std::cout << "  WRITE_FILE filename=output.dat" << std::endl;
    std::cout << "  STOP_SCOPE" << std::endl;
    std::cout << "============================================================\n" << std::endl;
}

int main(int argc, char *argv[])
{
    std::cout << "\n========== Oscilloscope System Starting ==========" << std::endl;
    // Check for help flag
    if (argc > 1 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))
    {
        printUsage(argv[0]);
        return 0;
    }

    // Initialize FTDI devices
    FtdiDevice readDevice("FTDI_Read");
    FtdiDevice writeDevice("FTDI_Write");

    readDevice.open();
    writeDevice.open();

    // Initialize oscilloscope components
    scpReader reader(readDevice);
    scpWriter writer(writeDevice);
    scpConfig scopeConfig;

    // Set default sample rate for threaded collection
    scopeConfig.setSampleRate(1000); // 1000 Hz = 1ms per sample

    scpController scopeController(&reader, &writer, &scopeConfig);

    // Initialize function generator components
    fgenConfig fgenCfg;
    scpWaveformGen waveGen(&fgenCfg);
    fgenController fgenCtrl(&writer, &fgenCfg, &waveGen);

    // Initialize command parser
    scpCommandParser parser(&scopeController, &fgenCtrl);

    // Parse commands
    bool commandsParsed = false;

    if (argc > 2 && std::string(argv[1]) == "-f")
    {
        // Parse from file
        std::string filename = argv[2];
        std::cout << "[Main] Parsing commands from file: " << filename << std::endl;
        commandsParsed = parser.parseFile(filename);
    }
    else if (argc > 1)
    {
        // Parse from command line
        std::cout << "[Main] Parsing commands from command line" << std::endl;
        commandsParsed = parser.parseCommandLine(argc, argv);
    }
    else
    {
        // No commands provided - show usage
        std::cerr << "[Main] ERROR: No commands provided" << std::endl;
        printUsage(argv[0]);
        readDevice.close();
        writeDevice.close();
        return 1;
    }

    if (!commandsParsed)
    {
        std::cerr << "[Main] ERROR: Failed to parse commands" << std::endl;
        readDevice.close();
        writeDevice.close();
        return 1;
    }

    // Display parsed commands
    parser.displayCommands();

    // Execute all commands
    parser.executeCommands();

    // Cleanup
    std::cout << "\n[Main] Cleaning up..." << std::endl;
    readDevice.close();
    writeDevice.close();

    std::cout << "[Main] ========== System shutdown complete ==========\n" << std::endl;
    return 0;
}