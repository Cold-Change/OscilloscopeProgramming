#ifndef FTDIDEVICE_H
#define FTDIDEVICE_H

#include <string>
#include <iostream>
#include <atomic>

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

private:
    static std::atomic<int> nextInstanceId;
    
    int instanceId;
    std::string id;        // Device identifier
    std::string label;     // Human-readable label
    bool openFlag;         // Connection status flag
};

#endif // FTDIDEVICE_H