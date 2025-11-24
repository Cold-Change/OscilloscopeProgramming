#include "FtdiDiagnostics.h"
#include <thread>
#include <chrono>

void FtdiDiagnostics::runFullDiagnostics(FtdiDevice& device)
{
    if (!device.isOpen()) {
        std::cerr << "Device must be open for diagnostics" << std::endl;
        return;
    }
    
    FT_HANDLE handle = device.getHandle();
    
    std::cout << "\n╔════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║        FTDI COMPREHENSIVE DIAGNOSTICS             ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════╝\n" << std::endl;
    
    testBitModes(handle);
    testPinStates(handle);
    readEEPROM(handle);
    testLoopback(handle);
    
    std::cout << "\n╔════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           DIAGNOSTICS COMPLETE                     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════╝\n" << std::endl;
}

void FtdiDiagnostics::testBitModes(FT_HANDLE handle)
{
    std::cout << "═══ TEST 1: BIT MODE CONFIGURATION ═══" << std::endl;
    
    // Test RESET mode
    std::cout << "Testing RESET mode (0x00)..." << std::endl;
    FT_STATUS status = FT_SetBitMode(handle, 0x00, 0x00);
    std::cout << "  Status: " << (status == FT_OK ? "✓ OK" : "✗ FAILED") << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Test Async Bit Bang INPUT
    std::cout << "Testing Async Bit Bang INPUT (mask=0x00, mode=0x01)..." << std::endl;
    status = FT_SetBitMode(handle, 0x00, 0x01);
    std::cout << "  Status: " << (status == FT_OK ? "✓ OK" : "✗ FAILED") << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Test Async Bit Bang OUTPUT
    std::cout << "Testing Async Bit Bang OUTPUT (mask=0xFF, mode=0x01)..." << std::endl;
    status = FT_SetBitMode(handle, 0xFF, 0x01);
    std::cout << "  Status: " << (status == FT_OK ? "✓ OK" : "✗ FAILED") << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Return to INPUT mode
    FT_SetBitMode(handle, 0x00, 0x01);
    std::cout << std::endl;
}

void FtdiDiagnostics::testPinStates(FT_HANDLE handle)
{
    std::cout << "═══ TEST 2: PIN STATE READING ═══" << std::endl;
    
    // Purge buffers
    FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Set to INPUT mode
    FT_SetBitMode(handle, 0x00, 0x01);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "Reading 10 samples in INPUT mode:" << std::endl;
    for (int i = 0; i < 10; i++) {
        uint8_t data;
        DWORD bytesRead;
        FT_STATUS status = FT_Read(handle, &data, 1, &bytesRead);
        
        if (status == FT_OK && bytesRead > 0) {
            std::cout << "  Sample " << std::setw(2) << (i+1) << ": 0x" 
                      << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(data) << " (binary: ";
            
            for (int bit = 7; bit >= 0; bit--) {
                std::cout << ((data & (1 << bit)) ? '1' : '0');
            }
            std::cout << ")" << std::dec << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Now test OUTPUT mode
    std::cout << "\nTesting OUTPUT mode (writing patterns):" << std::endl;
    FT_SetBitMode(handle, 0xFF, 0x01);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    uint8_t patterns[] = {0x00, 0x01, 0x55, 0xAA, 0xFF};
    const char* labels[] = {"All LOW", "DB0 HIGH", "Alternating 01", "Alternating 10", "All HIGH"};
    
    for (int i = 0; i < 5; i++) {
        DWORD written;
        FT_Write(handle, &patterns[i], 1, &written);
        std::cout << "  Wrote 0x" << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(patterns[i]) << " - " << labels[i] << std::dec << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    // Return to INPUT
    FT_SetBitMode(handle, 0x00, 0x01);
    std::cout << std::endl;
}

void FtdiDiagnostics::readEEPROM(FT_HANDLE handle)
{
    std::cout << "═══ TEST 3: EEPROM CONFIGURATION ═══" << std::endl;
    
    // Read first 16 words of EEPROM
    std::cout << "Reading EEPROM configuration:" << std::endl;
    
    for (int addr = 0; addr < 16; addr++) {
        WORD value;
        FT_STATUS status = FT_ReadEE(handle, addr, &value);
        
        if (status == FT_OK) {
            std::cout << "  EEPROM[0x" << std::hex << std::setw(2) << std::setfill('0') 
                      << addr << "] = 0x" << std::setw(4) << value << std::dec;
            
            // Interpret important addresses
            if (addr == 0x00) std::cout << " (Chip ID)";
            if (addr == 0x01) std::cout << " (Vendor ID)";
            if (addr == 0x02) std::cout << " (Product ID)";
            if (addr == 0x0A) {
                std::cout << " (Config: ";
                if (value & 0x0800) std::cout << "Pull-ups enabled";
                else std::cout << "Pull-downs enabled";
                std::cout << ")";
            }
            
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void FtdiDiagnostics::testLoopback(FT_HANDLE handle)
{
    std::cout << "═══ TEST 4: LOOPBACK TEST (if wired) ═══" << std::endl;
    std::cout << "To perform loopback test: Connect DB0 to DB1" << std::endl;
    std::cout << "Skipping automatic loopback (would require hardware setup)" << std::endl;
    std::cout << std::endl;
}