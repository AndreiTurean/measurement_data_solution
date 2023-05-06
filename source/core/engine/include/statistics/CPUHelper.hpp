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
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
            clock_t lastCPU, lastSysCPU, lastUserCPU;
            int numProcessors;
            float lastCPUUsageValue_ = 0.0f;
#elif defined(_WIN32)
            ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
            int numProcessors;
            HANDLE self;
#endif
        public:
            CPUHelper()
            {
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
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
#elif defined(_WIN32)

                SYSTEM_INFO sysInfo;
                FILETIME ftime, fsys, fuser;

                GetSystemInfo(&sysInfo);
                numProcessors = sysInfo.dwNumberOfProcessors;

                GetSystemTimeAsFileTime(&ftime);
                std::memcpy(&lastCPU, &ftime, sizeof(FILETIME));

                self = GetCurrentProcess();
                GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
                std::memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
                std::memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

#endif
            }
            float getCPUUsage()
            {
#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
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

                

#elif defined(_WIN32)
                FILETIME ftime, fsys, fuser;
                ULARGE_INTEGER now, sys, user;
                double percent;

                GetSystemTimeAsFileTime(&ftime);
                memcpy(&now, &ftime, sizeof(FILETIME));

                GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
                memcpy(&sys, &fsys, sizeof(FILETIME));
                memcpy(&user, &fuser, sizeof(FILETIME));
                percent = (sys.QuadPart - lastSysCPU.QuadPart) +
                    (user.QuadPart - lastUserCPU.QuadPart);
                percent /= (now.QuadPart - lastCPU.QuadPart);
                percent /= numProcessors;
                lastCPU = now;
                lastUserCPU = user;
                lastSysCPU = sys;
                percent *= 100;
#endif
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