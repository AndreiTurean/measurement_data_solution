#pragma once
#include <mutex>
#include <thread>
#include <cstdint>
#include <defs/Distribution.hpp>
using namespace std::chrono_literals;
namespace core
{
    namespace statistics
    {
        class DistributionStatistics:
            public DataDistributionStatistics
        {
            uint64_t processedPackagesPerSecond_;
            uint64_t maxProcessedPackagesPerSecond_;
            uint64_t failedPackagesCnt_;
            uint64_t processedPackagesCnt_;
            bool alive_;

            std::mutex countingMtx_;
            std::mutex updateMtx_;
            std::unique_ptr<std::thread> countingThread_;

            void count()
            {
                while(true)
                {
                    {
                        std::lock_guard<std::mutex> lock(countingMtx_);
                        std::lock_guard<std::mutex> lock2(updateMtx_);

                        if(!alive_)
                        {
                            return;
                        }
                        processedPackagesPerSecond_ = processedPackagesCnt_ + failedPackagesCnt_;
                        //maxProcessedPackagesPerSecond_ = std::max(maxProcessedPackagesPerSecond_, processedPackagesPerSecond_);
        
                        processedPackagesCnt_ = 0;
                        failedPackagesCnt_ = 0;
                    }

                    std::this_thread::sleep_for(999ms);
                }
            }
        public:
            DistributionStatistics():
                processedPackagesPerSecond_(0),
                maxProcessedPackagesPerSecond_(0),
                failedPackagesCnt_(0),
                processedPackagesCnt_(0),
                alive_(true)
            {
                countingThread_ = std::make_unique<std::thread>(&DistributionStatistics::count, this);
            }

            virtual ~DistributionStatistics()
            {
                {
                    std::lock_guard<std::mutex> lock(countingMtx_);
                    alive_ = false;
                }

                countingThread_->join();
                countingThread_.reset();
            }

            void update(bool processed = true)
            {
                std::lock_guard<std::mutex> lock(countingMtx_);
                processed ? processedPackagesCnt_++ : failedPackagesCnt_++;
            }

            virtual const uint64_t& getNumberOfProcessedPackagesPerSecond()
            {
                std::lock_guard<std::mutex> lock2(updateMtx_);
                return processedPackagesPerSecond_;
            }
            virtual const uint64_t& getMaximumProcessedPackagesPerSecond()
            {
                std::lock_guard<std::mutex> lock2(updateMtx_);
                return processedPackagesPerSecond_;
            }
        };
    }
}