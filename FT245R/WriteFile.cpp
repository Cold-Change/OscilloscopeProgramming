#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

class FtdiDevice {
public:
    explicit FtdiDevice(std::string id) : _id(std::move(id)), _open(false) {}
    bool open() {
        _open = true;
        std::cout << "[FTDI] " << _id << " opened.\n";
        return true;
    }
    void close() {
        _open = false;
        std::cout << "[FTDI] " << _id << " closed.\n";
    }
    bool isOpen() const { return _open; }

private:
    std::string _id;
    bool _open;
};

// ---------------- reqfWrite Class ----------------
class WriteFile {
public:
    explicit WriteFile(FtdiDevice& device) : dev(device) {}

    void writeData(const std::string& filename, const std::vector<uint8_t>& data) {
        if (!dev.isOpen()) {
            std::cerr << "[ERROR] Write device not open.\n";
            return;
        }
        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out) {
            std::cerr << "[ERROR] Cannot open output file: " << filename << "\n";
            return;
        }
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "[WRITE] " << data.size() << " bytes written to " << filename << "\n";
    }

private:
    FtdiDevice& dev;
};

// make the class available to other files
#ifndef WRITEFILE_H
#define WRITEFILE_H
extern class WriteFile;
extern class FtdiDevice;
#endif
