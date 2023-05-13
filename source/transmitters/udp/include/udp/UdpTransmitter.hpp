#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/MeasurementObjectDefs.hpp>
#include <defs/Receiver.hpp>
#include <set>
#include <mutex>
#include <defs/Log.hpp>

namespace pcpp
{
    class RawPacket;
    class PcapLiveDevice;
    class PcapLiveDeviceList;
}

namespace transmitters
{
    class UDP_TRANSMITTER_API UdpTransmitter :
        public MeasurementObject,
        public DataReceiverObject,
        public ObjectControl,
        public GuiControlIfc
    {
        InterfaceAccess* interfaceAccess_;
        LoggingInterface* loggingInterface_;
        pcpp::PcapLiveDevice* currentDevice_;
        uint8_t instanceNb_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        std::string ipAdress_;
        std::string ifcName_;
        pcpp::PcapLiveDeviceList* devList_;

    public:
        UdpTransmitter(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name);
        virtual ~UdpTransmitter();

        //! MeasurementObject interface implementation
        const uint8_t& getInstanceNumber();
        const uint64_t& getHandle();
        const MeasurementObjectType& getType();
        const std::string& getName();

        //! DataReceiverObject interface implementation
        bool validatePackage(DataPackageCPtr package);

        void initializeObject() override;
        void terminateObject() override;

        void show(ImGuiContext* ctx) override;
    };
}

extern "C" UDP_TRANSMITTER_API MeasurementObjectPtr createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
{
    return new transmitters::UdpTransmitter(interfaceAccess, instanceNb, name);
}