#ifndef SCPDATACOLLECTOR_H
#define SCPDATACOLLECTOR_H

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>
#include "scpReader.h"
#include "scpConfig.h"

namespace scp {

/**
 * Class: scpDataCollector (CLS-014)
 * Responsibility: Continuously collect data in separate thread with real-time display
 * Association: Uses scpReader (1:1), Uses scpConfig (1:1)
 */
class DataCollector {
public:
    using Clock = std::chrono::steady_clock;
    
    struct CollectionResult {
        std::vector<uint8_t> data;
        std::uint64_t samplesCollected;
        std::chrono::milliseconds totalTime;
    };

    explicit DataCollector(scpReader* reader, scpConfig* config, std::mutex& out_mx);
    ~DataCollector();

    // Start collection with specified duration
    void start(std::chrono::seconds duration);
    
    // Stop collection early
    void stop();
    
    // Wait for collection to finish
    void join();
    
    // Check if collection is done
    bool done() const { return done_.load(std::memory_order_relaxed); }
    
    // Get collected data (call after join)
    CollectionResult getResult();
    
    // Get deadline for coordination with timer
    Clock::time_point deadline() const { return deadline_; }

private:
    void run_();
    void displayProgress_(std::uint64_t samples, unsigned lastValue);

    scpReader* reader_;
    scpConfig* config_;
    std::mutex& out_mx_;
    std::thread th_;
    std::atomic<bool> stop_{false};
    std::atomic<bool> done_{false};
    Clock::time_point deadline_{};
    Clock::time_point startTime_{};
    std::chrono::seconds duration_;
    
    // Thread-safe data storage
    std::vector<uint8_t> collectedData_;
    std::mutex dataMutex_;
    std::uint64_t totalSamples_{0};
};

} // namespace scp

#endif // SCPDATACOLLECTOR_H