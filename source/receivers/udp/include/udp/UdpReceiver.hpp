#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/Distribution.hpp>
#include <defs/Log.hpp>

namespace pcpp
{
    class RawPacket;
    class PcapLiveDevice;
    class PcapLiveDeviceList;
}

namespace receivers
{
    class UDP_RECEIVER_API UdpReceiver :
        public MeasurementObject,
        public ObjectControl,
        public GuiControlIfc
    {
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        DataDistribution* dataDistributionInterface_;
        std::string ifcName_;
        pcpp::PcapLiveDevice* currentDevice_;

    private:
        std::string ipAdress_;
        std::thread processingThread_;
        static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie);
        pcpp::PcapLiveDeviceList* devList_;
        LoggingInterface* loggingInterface_;

    public:
        UdpReceiver(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name);
        virtual ~UdpReceiver();
        const uint8_t& getInstanceNumber() override;
        const uint64_t& getHandle() override;
        const MeasurementObjectType& getType() override;
        const std::string& getName() override;
        void initializeObject() override;
        void terminateObject() override;
        void show(ImGuiContext* ctx) override;
    };
}

extern "C" UDP_RECEIVER_API MeasurementObjectPtr createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
{
    return new receivers::UdpReceiver(interfaceAccess, instanceNb, name);
}
