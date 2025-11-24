#include "FtdiDevice.h"
#include <thread>        // ADD THIS LINE
#include <chrono>        // ADD THIS LINE

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
    
<<<<<<< Updated upstream
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
=======
<<<<<<< Updated upstream
    openFlag = true;
    std::cout << label << " opened successfully" << std::endl;
=======
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
    
    // STEP 8: Set bit mode for INPUT reading
    // CRITICAL: 0x00 mask = ALL PINS AS INPUTS
    std::cout << label << " Configuring pins for INPUT mode..." << std::endl;
    ftStatus = FT_SetBitMode(ftHandle, 0x00, 0x01);
    
    if (ftStatus != FT_OK)
    {
        std::cerr << label << " ❌ ERROR: Failed to set bit mode (code: " << ftStatus << ")" << std::endl;
        FT_Close(ftHandle);
        ftHandle = nullptr;
        return false;
    }
    else
    {
        std::cout << label << " ✓ Asynchronous Bit Bang INPUT mode enabled" << std::endl;
        std::cout << label << " ✓ All pins (DB0-DB7) configured as INPUTS" << std::endl;
        std::cout << label << " ✓ Pin states will reflect external voltage levels" << std::endl;
    }
    
    // STEP 9: Brief delay to let configuration stabilize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    openFlag = true;
    std::cout << label << " ========================================" << std::endl;
    std::cout << label << " 🎉 FTDI DEVICE READY FOR DATA ACQUISITION!" << std::endl;
    std::cout << label << " ========================================" << std::endl;
    std::cout << label << " Pin Configuration:" << std::endl;
    std::cout << label << "   DB0 (Pin 10) = INPUT - Connect signal here" << std::endl;
    std::cout << label << "   DB1 (Pin 9)  = INPUT" << std::endl;
    std::cout << label << "   DB2 (Pin 8)  = INPUT" << std::endl;
    std::cout << label << "   DB3 (Pin 7)  = INPUT" << std::endl;
    std::cout << label << "   DB4 (Pin 6)  = INPUT" << std::endl;
    std::cout << label << "   DB5 (Pin 5)  = INPUT" << std::endl;
    std::cout << label << "   DB6 (Pin 4)  = INPUT" << std::endl;
    std::cout << label << "   DB7 (Pin 3)  = INPUT" << std::endl;
    std::cout << label << " ========================================" << std::endl;
>>>>>>> Stashed changes
>>>>>>> Stashed changes
    
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

void FtdiDevice::diagnosePullUps()
{
    if (!isOpen()) {
        std::cerr << label << " Device must be open to diagnose" << std::endl;
        return;
    }
    
    std::cout << "\n" << label << " ========== PULL-UP DIAGNOSTICS ==========" << std::endl;
    
    // Read EEPROM word 0x0A (contains pull-up configuration for FT232R)
    WORD eepromValue = 0;
    FT_STATUS status = FT_ReadEE(ftHandle, 0x0A, &eepromValue);
    
    if (status == FT_OK) {
        std::cout << label << " EEPROM[0x0A] = 0x" << std::hex << eepromValue << std::dec << std::endl;
        
        if (eepromValue & 0x0800) {
            std::cout << label << " ⚠️  CBUS pull-downs DISABLED (pull-ups may be active)" << std::endl;
        } else {
            std::cout << label << " ✓ CBUS pull-downs enabled" << std::endl;
        }
    } else {
        std::cout << label << " WARNING: Could not read EEPROM (status: " << status << ")" << std::endl;
    }
    
    std::cout << label << " ========================================\n" << std::endl;
}

void FtdiDevice::testPinControl()
{
    if (!isOpen()) return;
    
    std::cout << "\n" << label << " ========== PIN CONTROL TEST ==========" << std::endl;
    
    // Set all pins as OUTPUT and drive LOW
    FT_SetBitMode(ftHandle, 0xFF, 0x01);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Write 0x00 (all LOW)
    DWORD written;
    uint8_t testData = 0x00;
    FT_Write(ftHandle, &testData, 1, &written);
    
    std::cout << label << " Wrote 0x00 (all pins LOW)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Write 0xFF (all HIGH)
    testData = 0xFF;
    FT_Write(ftHandle, &testData, 1, &written);
    std::cout << label << " Wrote 0xFF (all pins HIGH)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Switch back to INPUT
    FT_SetBitMode(ftHandle, 0x00, 0x01);
    std::cout << label << " Switched back to INPUT mode" << std::endl;
    std::cout << label << " ========================================\n" << std::endl;
}