#ifndef FTDIDEVICE_H
#define FTDIDEVICE_H

#include <string>
#include <iostream>

class FtdiDevice
{
public:
    explicit FtdiDevice(const std::string& id);
    
    ~FtdiDevice();
    
    bool open();
    
    void close();
    
    bool isOpen() const;
    
    std::string getDeviceId() const;

private:
    std::string id;        // Device identifier
    bool openFlag;         // Connection status flag
};

#endif // FTDIDEVICE_H
