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

// ---------------- reqfRead Class ----------------
class ReadFile {
public:
    explicit ReadFile(FtdiDevice& device) : dev(device) {}

    std::vector<uint8_t> readData(const std::string& filename) {
        if (!dev.isOpen()) {
            std::cerr << "[ERROR] Read device not open.\n";
            return {};
        }
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            std::cerr << "[ERROR] Cannot open input file: " << filename << "\n";
            return {};
        }
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
        std::cout << "[READ] " << data.size() << " bytes read from " << filename << "\n";
        return data;
    }

private:
    FtdiDevice& dev;
};

// make the class available to other files
#ifndef READFILE_H
#define READFILE_H
extern class ReadFile;
extern class FtdiDevice;
#endif
