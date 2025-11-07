#include "FtdiDevice.h"

/**
 * Constructor - Initialize FTDI device with given ID
 * @param id Device identifier string
 */
FtdiDevice::FtdiDevice(const std::string& id)
    : id(id), openFlag(false)
{
    std::cout << "[FtdiDevice] Device '" << id << "' created" << std::endl;
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
    std::cout << "[FtdiDevice] Device '" << id << "' destroyed" << std::endl;
}

/**
 * Open the FTDI device connection
 * In a real implementation, this would:
 * - Initialize USB communication
 * - Configure device parameters
 * - Establish connection with hardware
 * 
 * @return true if successfully opened, false otherwise
 */
bool FtdiDevice::open()
{
    if (openFlag)
    {
        std::cout << "[FtdiDevice] Device '" << id << "' is already open" << std::endl;
        return true;
    }
    
    // In real implementation, this would call FTDI library functions
    // For example: ftdi_usb_open_desc() or similar
    // Here we simulate successful opening
    
    openFlag = true;
    std::cout << "[FtdiDevice] FTDI device '" << id << "' opened successfully" << std::endl;
    
    return true;
}

/**
 * Close the FTDI device connection
 * In a real implementation, this would:
 * - Flush any pending data
 * - Release USB resources
 * - Close the device handle
 */
void FtdiDevice::close()
{
    if (!openFlag)
    {
        std::cout << "[FtdiDevice] Device '" << id << "' is already closed" << std::endl;
        return;
    }
    
    // In real implementation, this would call FTDI library functions
    // For example: ftdi_usb_close() or similar
    
    openFlag = false;
    std::cout << "[FtdiDevice] FTDI device '" << id << "' closed successfully" << std::endl;
}

/**
 * Check if device is currently open
 * @return true if device is open, false otherwise
 */
bool FtdiDevice::isOpen() const
{
    return openFlag;
}

/**
 * Get the device identifier
 * @return Device ID string
 */
std::string FtdiDevice::getDeviceId() const
{
    return id;
}
