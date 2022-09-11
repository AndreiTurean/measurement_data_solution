#pragma once
#include <visibility.h>
#include <MiniMTS.hpp>
#include <Distribution.hpp>
#include <memory>
#include <Distribution.hpp>
#include <thread>
#include <mutex>

namespace transmitters
{
    class Dummy :
        public InterfaceAccess,
        public MeasurementObject,
        public DataSenderObject
    {
        DataDistributionPtr dataDistributinonPtr_;
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        DataDistribution* dataDistributionInterface_;

    private:
        std::mutex processingMtx_;
        bool isProcessing_;
        std::unique_ptr<std::thread> processingThread_;
        void doFSMProcessing();
        
    public:
        Dummy(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name);
        ~Dummy();
        virtual void* getInterface(const std::string& interfaceName);
        virtual void startProcessing();
        virtual void endProcessing();
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();
    };
}

// the types of the class factories
typedef std::shared_ptr<MeasurementObject> createMO_t(InterfaceAccess*, const uint8_t, uint64_t, const char*);

#if defined _WIN32
extern "C" std::shared_ptr<MeasurementObject> DUMMY_API createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name);
#else
extern "C" std::shared_ptr<MeasurementObject> createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name);
#endif