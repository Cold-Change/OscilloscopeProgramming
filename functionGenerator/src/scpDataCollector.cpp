#include "scpDataCollector.h"
#include <iostream>
#include <iomanip>

namespace scp {

DataCollector::DataCollector(scpReader* reader, scpConfig* config, std::mutex& out_mx)
    : reader_(reader), config_(config), out_mx_(out_mx) {}

DataCollector::~DataCollector() {
    stop();
    join();
}

void DataCollector::start(std::chrono::seconds duration) {
    duration_ = duration;
    startTime_ = Clock::now();
    deadline_ = startTime_ + duration_;
    
    {
        std::lock_guard<std::mutex> lk(out_mx_);
        std::cout << "\n[DataCollector] Starting collection for " << duration_.count() 
                  << " seconds" << std::endl;
        std::cout << "[DataCollector] Sample rate: " << config_->getSampleRate() << " Hz" << std::endl;
    }
    
    th_ = std::thread(&DataCollector::run_, this);
}

void DataCollector::stop() {
    stop_.store(true, std::memory_order_relaxed);
}

void DataCollector::join() {
    if (th_.joinable()) th_.join();
}

DataCollector::CollectionResult DataCollector::getResult() {
    std::lock_guard<std::mutex> lk(dataMutex_);
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        Clock::now() - startTime_);
    return {collectedData_, totalSamples_, totalTime};
}

void DataCollector::displayProgress_(std::uint64_t samples, unsigned lastValue) {
    std::lock_guard<std::mutex> lk(out_mx_);
    
    // Calculate elapsed time
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        now - startTime_).count();
    auto total = duration_.count();
    
    // Display format: scrolling data with progress bar
    std::cout << "[DataCollector] Samples: " << std::setw(8) << samples 
              << " | Last: 0x" << std::hex << std::setw(2) 
              << std::setfill('0') << static_cast<int>(lastValue) 
              << std::dec << std::setfill(' ')
              << " (" << std::setw(3) << static_cast<int>(lastValue) << ")"
              << " | ";
    
    // Progress bar
    int barWidth = 20;
    int pos = (elapsed * barWidth) / total;
    
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << elapsed << "s/" << total << "s";
    
    // Show sample rate
    if (elapsed > 0) {
        double rate = static_cast<double>(samples) / elapsed;
        std::cout << " | Rate: " << std::fixed << std::setprecision(1) << rate << " Hz";
    }
    
    std::cout << "\r" << std::flush;
}

void DataCollector::run_() {
    std::uint64_t iterations = 0;
    
    // Calculate sample interval based on configured sample rate
    auto sampleInterval = std::chrono::microseconds(
        config_->getSampleRate() > 0 ? 1000000 / config_->getSampleRate() : 1000);
    
    // Display interval - update every 100ms for smooth scrolling
    auto displayInterval = std::chrono::milliseconds(100);
    auto nextDisplay = Clock::now() + displayInterval;
    
    uint8_t lastSampleValue = 0;
    
    while (!stop_.load(std::memory_order_relaxed)) {
        auto now = Clock::now();
        if (now >= deadline_) break;
        
        // Collect one sample
        std::vector<uint8_t> sample = reader_->readSamples(1);
        
        if (!sample.empty()) {
            lastSampleValue = sample[0];
            
            {
                std::lock_guard<std::mutex> lk(dataMutex_);
                collectedData_.insert(collectedData_.end(), sample.begin(), sample.end());
                totalSamples_ += sample.size();
            }
            
            ++iterations;
            
            // Update display at regular intervals
            if (now >= nextDisplay) {
                displayProgress_(totalSamples_, lastSampleValue);
                nextDisplay += displayInterval;
                if (nextDisplay > deadline_) nextDisplay = deadline_;
            }
        }
        
        // Sleep for sample interval
        std::this_thread::sleep_for(sampleInterval);
    }
    
    // Final display update
    displayProgress_(totalSamples_, lastSampleValue);
    
    {
        std::lock_guard<std::mutex> lk(out_mx_);
        std::cout << "\n[DataCollector] Collection complete. Total samples: " 
                  << totalSamples_ << std::endl;
    }
    
    done_.store(true, std::memory_order_relaxed);
}

} // namespace scp