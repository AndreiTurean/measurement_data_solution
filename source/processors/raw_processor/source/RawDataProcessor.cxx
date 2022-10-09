#include <RawDataProcessor.hpp>
#include <iostream>

namespace processors
{
    RawDataProcessor::RawDataProcessor(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name):
        interfaceAccess_(interfaceAccess),
        instanceNb_(nb),
        handle_(INVALID_HANDLE),
        name_(name),
        type_(MeasurementObjectType::data_receiver)
    {
        handle_ = (instanceNb_ + 1);
        handle_ = handle_ << 0x8;
    }
    RawDataProcessor::~RawDataProcessor()
    {
        interfaceAccess_ = nullptr;
        subjects_.clear();
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

    void RawDataProcessor::initializeObject()
    {
        subjects_.clear();
    }
    void RawDataProcessor::terminateObject()
    {
        subjects_.clear();
        delete this;
    }
}