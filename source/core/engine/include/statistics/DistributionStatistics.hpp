#pragma once
#include <cstdint>

namespace core
{
    namespace statistics
    {
        class DistributionStatistics
        {
            size_t failedCnt_;
            size_t passedCnt_;
        public:
            DistributionStatistics()
            {
                failedCnt_ = 0;
                passedCnt_ = 0;
            }

            void update(bool pass = true)
            {
                pass ? passedCnt_++ : passedCnt_++;
            }

            void reset(size_t& pass, size_t& fail)
            {
                pass = passedCnt_;
                fail = failedCnt_;

                reset();
            }

            void reset()
            {
                failedCnt_ = 0;
                passedCnt_ = 0;
            }
            size_t getAmountOfProcessedPackagesPerSecond()
            {
                return 0;
            }
        };
    }
}