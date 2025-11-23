#include "FtdiDevice.h"

// Initialize static counter
std::atomic<int> FtdiDevice::nextInstanceId(1);

/**
 * Constructor - Initialize FTDI device with given ID
 * @param id Device identifier string
 */
FtdiDevice::FtdiDevice(const std::string& id)
    : instanceId(nextInstanceId++), id(id), openFlag(false)
{
    label = "[FTDI-" + std::to_string(instanceId) + "] " + id;
    std::cout << label << " created" << std::endl;
}

/**
 * Destructor - Ensure device is closed before destruction
 */
FtdiDevice::~FtdiDevice()
{
    if (openFlag)
    {
        close();
    }
    std::cout << label << " destroyed" << std::endl;
}

/**
 * Open the FTDI device connection
 */
bool FtdiDevice::open()
{
    if (openFlag)
    {
        std::cout << label << " is already open" << std::endl;
        return true;
    }
    
    openFlag = true;
    std::cout << label << " opened successfully" << std::endl;
    
    return true;
}

/**
 * Close the FTDI device connection
 */
void FtdiDevice::close()
{
    if (!openFlag)
    {
        std::cout << label << " is already closed" << std::endl;
        return;
    }
    
    openFlag = false;
    std::cout << label << " closed successfully" << std::endl;
}

/**
 * Check if device is currently open
 */
bool FtdiDevice::isOpen() const
{
    return openFlag;
}

/**
 * Get the device identifier
 */
std::string FtdiDevice::getDeviceId() const
{
    return id;
}