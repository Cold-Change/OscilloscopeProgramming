#ifndef FTDIDIAGNOSTICS_H
#define FTDIDIAGNOSTICS_H

#include "FtdiDevice.h"
#include <iostream>
#include <iomanip>

class FtdiDiagnostics
{
public:
    static void runFullDiagnostics(FtdiDevice& device);
    
private:
    static void testBitModes(FT_HANDLE handle);
    static void testPinStates(FT_HANDLE handle);
    static void readEEPROM(FT_HANDLE handle);
    static void testLoopback(FT_HANDLE handle);
};

#endif