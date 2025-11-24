#include "FtdiDevice.h"

// Initialize static counter
std::atomic<int> FtdiDevice::nextInstanceId(1);

FtdiDevice::FtdiDevice(const std::string& id)
    : instanceId(nextInstanceId++), id(id), openFlag(false), ftHandle(nullptr)
{
    label = "[FTDI-" + std::to_string(instanceId) + "] " + id;
    std::cout << label << " created" << std::endl;
}

FtdiDevice::~FtdiDevice()
{
    if (openFlag)
    {
        close();
    }
    std::cout << label << " destroyed" << std::endl;
}

bool FtdiDevice::open()
{
    if (openFlag)
    {
        std::cout << label << " is already open" << std::endl;
        return true;
    }
    
    FT_STATUS ftStatus;
    DWORD numDevices = 0;
    
    // STEP 1: Check how many FTDI devices are connected
    ftStatus = FT_CreateDeviceInfoList(&numDevices);
    
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ❌ ERROR: Failed to query FTDI devices. Status code: " << ftStatus << std::endl;
        std::cerr << label << " Make sure FTDI drivers are installed!" << std::endl;
        return false;
    }
    
    if (numDevices == 0)
    {
        std::cerr << label << " ❌ ERROR: NO FTDI DEVICES CONNECTED!" << std::endl;
        std::cerr << label << " Please connect an FTDI device and try again." << std::endl;
        std::cerr << label << " Supported devices: FT232R, FT245R, FT2232H, FT232H" << std::endl;
        return false;
    }
    
    std::cout << label << " ✓ Found " << numDevices << " FTDI device(s) connected" << std::endl;
    
    // STEP 2: Get device information
    FT_DEVICE_LIST_INFO_NODE *devInfo = new FT_DEVICE_LIST_INFO_NODE[numDevices];
    ftStatus = FT_GetDeviceInfoList(devInfo, &numDevices);
    
    if (ftStatus == FT_OK && numDevices > 0)
    {
        std::cout << label << " Device Info:" << std::endl;
        std::cout << label << "   - Type: " << devInfo[0].Type << std::endl;
        std::cout << label << "   - Serial: " << devInfo[0].SerialNumber << std::endl;
        std::cout << label << "   - Description: " << devInfo[0].Description << std::endl;
    }
    
    delete[] devInfo;
    
    // STEP 3: Try to open device by index (0 = first device)
    ftStatus = FT_Open(0, &ftHandle);
    
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ❌ ERROR: Failed to open FTDI device!" << std::endl;
        std::cerr << label << " Error code: " << ftStatus << std::endl;
        
        // Detailed error messages
        switch(ftStatus)
        {
            case FT_INVALID_HANDLE:
                std::cerr << label << " Reason: Invalid device handle" << std::endl;
                break;
            case FT_DEVICE_NOT_FOUND:
                std::cerr << label << " Reason: Device not found" << std::endl;
                break;
            case FT_DEVICE_NOT_OPENED:
                std::cerr << label << " Reason: Device not opened" << std::endl;
                break;
            case FT_IO_ERROR:
                std::cerr << label << " Reason: I/O error" << std::endl;
                break;
            case FT_INSUFFICIENT_RESOURCES:
                std::cerr << label << " Reason: Insufficient system resources" << std::endl;
                break;
            default:
                std::cerr << label << " Reason: Unknown error" << std::endl;
                break;
        }
        
        ftHandle = nullptr;
        return false;
    }
    
    std::cout << label << " ✓ Device opened successfully" << std::endl;
    
    // STEP 4: Reset the device
    ftStatus = FT_ResetDevice(ftHandle);
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ⚠ WARNING: Failed to reset device (code: " << ftStatus << ")" << std::endl;
    }
    else
    {
        std::cout << label << " ✓ Device reset successfully" << std::endl;
    }
    
    // STEP 5: Purge USB buffers
    ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ⚠ WARNING: Failed to purge buffers (code: " << ftStatus << ")" << std::endl;
    }
    else
    {
        std::cout << label << " ✓ USB buffers purged" << std::endl;
    }
    
    // STEP 6: Set USB transfer sizes (4KB for optimal performance)
    ftStatus = FT_SetUSBParameters(ftHandle, 4096, 4096);
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ⚠ WARNING: Failed to set USB parameters (code: " << ftStatus << ")" << std::endl;
    }
    else
    {
        std::cout << label << " ✓ USB parameters configured (4KB buffers)" << std::endl;
    }
    
    // STEP 7: Set timeouts (500ms read, 500ms write)
    ftStatus = FT_SetTimeouts(ftHandle, 500, 500);
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ⚠ WARNING: Failed to set timeouts (code: " << ftStatus << ")" << std::endl;
    }
    else
    {
        std::cout << label << " ✓ Timeouts set (500ms R/W)" << std::endl;
    }
    
    // STEP 8: Set bit mode for Asynchronous Bit Bang (0x01)
    // This is the default mode for FT245R-style operation
    ftStatus = FT_SetBitMode(ftHandle, 0xFF, 0x01);
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ❌ ERROR: Failed to set bit mode (code: " << ftStatus << ")" << std::endl;
        FT_Close(ftHandle);
        ftHandle = nullptr;
        return false;
    }
    else
    {
        std::cout << label << " ✓ Asynchronous Bit Bang mode enabled" << std::endl;
    }
    
    openFlag = true;
    std::cout << label << " ========================================" << std::endl;
    std::cout << label << " 🎉 FTDI DEVICE READY FOR OPERATION!" << std::endl;
    std::cout << label << " ========================================" << std::endl;
    
    return true;
}

void FtdiDevice::close()
{
    if (!openFlag)
    {
        std::cout << label << " is already closed" << std::endl;
        return;
    }
    
    if (ftHandle != nullptr)
    {
        // Reset bit mode before closing
        FT_SetBitMode(ftHandle, 0x00, 0x00);
        
        FT_Close(ftHandle);
        ftHandle = nullptr;
        std::cout << label << " ✓ Device closed successfully" << std::endl;
    }
    
    openFlag = false;
}

bool FtdiDevice::isOpen() const
{
    return openFlag;
}

std::string FtdiDevice::getDeviceId() const
{
    return id;
}