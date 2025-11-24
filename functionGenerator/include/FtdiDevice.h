#ifndef FTDIDEVICE_H
#define FTDIDEVICE_H

#include <string>
#include <iostream>
#include <atomic>
#include "ftd2xx.h"  // ADD THIS - FTDI library

/**
 * Class: FtdiDevice (CLS-001)
 * Responsibility: Manage FTDI device connection lifecycle
 */
class FtdiDevice
{
public:
    explicit FtdiDevice(const std::string& id);
    
    ~FtdiDevice();
    
    bool open();
    
    void close();
    
    bool isOpen() const;
    
    std::string getDeviceId() const;
    
    int getInstanceId() const { return instanceId; }
    
    std::string getLabel() const { return label; }
    
    // NEW: Get actual FTDI handle for read/write operations
    FT_HANDLE getHandle() const { return ftHandle; }

private:
    static std::atomic<int> nextInstanceId;
    
    int instanceId;
    std::string id;        // Device identifier
    std::string label;     // Human-readable label
    bool openFlag;         // Connection status flag
    
    // NEW: Actual FTDI handle
    FT_HANDLE ftHandle;
};

#endif // FTDIDEVICE_H