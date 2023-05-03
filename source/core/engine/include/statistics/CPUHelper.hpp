#pragma once

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"

#elif defined(_WIN32)
#include <windows.h>
#endif

namespace core
{
    namespace helper
    {
        class CPUHelper
        {
            clock_t lastCPU, lastSysCPU, lastUserCPU;
            int numProcessors;
            float lastCPUUsageValue_ = 0.0f;
        public:
            CPUHelper()
            {
                FILE* file;
                struct tms timeSample;
                char line[128];

                lastCPU = times(&timeSample);
                lastSysCPU = timeSample.tms_stime;
                lastUserCPU = timeSample.tms_utime;

                file = fopen("/proc/cpuinfo", "r");
                numProcessors = 0;
                while(fgets(line, 128, file) != NULL){
                    if (strncmp(line, "processor", 9) == 0) numProcessors++;
                }
                fclose(file);
            }
            float getCPUUsage()
            {
                struct tms timeSample;
                clock_t now;
                float percent;

                now = times(&timeSample);
                if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
                    timeSample.tms_utime < lastUserCPU){
                    //Overflow detection. Just skip this value.
                    percent = lastCPUUsageValue_;
                }
                else{
                    percent = (timeSample.tms_stime - lastSysCPU) +
                        (timeSample.tms_utime - lastUserCPU);
                    percent /= (now - lastCPU);
                    percent /= numProcessors;
                    percent *= 100;
                }
                lastCPU = now;
                lastSysCPU = timeSample.tms_stime;
                lastUserCPU = timeSample.tms_utime;

                if(percent == static_cast<float>(0) || percent > static_cast<float>(100))
                {
                    percent = lastCPUUsageValue_;
                }
                else
                {
                    lastCPUUsageValue_ = percent;
                }

                return percent;
            };
        };
    }
}