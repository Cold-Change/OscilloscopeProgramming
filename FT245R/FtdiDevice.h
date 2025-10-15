#ifndef FTDIDEVICE_H
#define FTDIDEVICE_H

#include <string>
#include <iostream>

class FtdiDevice {
public:
    explicit FtdiDevice(const std::string& id)
        : id(id), openFlag(false) {}

    bool open() {
        openFlag = true;
        std::cout << "FTDI device " << id << " opened." << std::endl;
        return true;
    }

    void close() {
        openFlag = false;
        std::cout << "FTDI device " << id << " closed." << std::endl;
    }

    bool isOpen() const {
        return openFlag;
    }

private:
    std::string id;
    bool openFlag;
};

#endif
