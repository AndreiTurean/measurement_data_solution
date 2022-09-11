#include <RawDataProcessor.hpp>
#include <iostream>

namespace processors
{
        RawDataProcessor::RawDataProcessor(InterfaceAccess* interfaceAccess, uint8_t nb, uint64_t handle, const std::string& name):
            interfaceAccess_(interfaceAccess),
            instanceNb_(nb),
            handle_(handle),
            name_(name),
            type_(MeasurementObjectType::data_receiver)
        {

        }
        RawDataProcessor::~RawDataProcessor()
        {
        }

        const uint8_t& RawDataProcessor::getInstanceNumber()
        {
            return instanceNb_;
        }
        const uint64_t& RawDataProcessor::getHandle()
        {
            return handle_;
        }
        const MeasurementObjectType& RawDataProcessor::getType()
        {
            return type_;
        }
        const std::string& RawDataProcessor::getName()
        {
            return name_;
        }

        bool RawDataProcessor::validatePackage(DataPackageCPtr pkg)
        {
            for(auto const& subject : subjects_)
            {
                subject->notify(pkg);
            }
            return true;
        }

        void* RawDataProcessor::getInterface(const std::string& interfaceName)
        {
            if(interfaceName == "ReceiverSinkManager")
            {
                return dynamic_cast<ReceiverSinkManager*>(this);
            }
            if(interfaceAccess_)
            {
                return interfaceAccess_->getInterface(interfaceName);
            }
            return nullptr;
        }

        bool RawDataProcessor::registerToReceiverSink(NotifySubjects* subject)
        {
            return subjects_.insert(subject).second;

        }
        bool RawDataProcessor::unregisterToReceiverSink(NotifySubjects* subject)
        {
            auto it = subjects_.find(subject);

            if(it == subjects_.end())
            {
                return false;
            }
            subjects_.erase(it);
            return true;
        }
}

std::shared_ptr<MeasurementObject> createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const char* name)
{
	return std::make_shared<processors::RawDataProcessor>(interfaceAccess,instanceNb,handle,name);
}