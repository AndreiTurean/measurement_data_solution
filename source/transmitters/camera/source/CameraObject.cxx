#include <transmitters/CameraObject.hpp>

namespace transmitters
{
    CameraObject::CameraObject(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name):
        interfaceAccess_(interfaceAccess),
        instanceNumber_(instanceNb),
        handle_(INVALID_HANDLE),
        name_(name),
        type_(MeasurementObjectType::data_source),
        isProcessing_(true)
    {
        handle_ = (instanceNb + 1);
        handle_ = handle_ << 0x8 << 0x8;

        propertyTable_["Is processing"] = "No";
    }

    CameraObject::~CameraObject()
    {
        endProcessing();
    }
    void* CameraObject::getInterface(const std::string&)
    {
        return nullptr;
    }
    void CameraObject::startProcessing()
    {
        if(interfaceAccess_)
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            dataDistributionInterface_ = reinterpret_cast<DataDistribution*>(interfaceAccess_->getInterface("DataDistribution"));
            processingThread_ = std::make_unique<std::thread>(&transmitters::CameraObject::doFSMProcessing, this);
            propertyTable_["Is processing"] = "Yes";
            propertyTable_["Status"] = "Ok";
        }
        else
        {
            propertyTable_["Is processing"] = "No";
            propertyTable_["Status"] = "Critical";
        }
    }
    void CameraObject::endProcessing()
    {
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            isProcessing_ = false;
        }

        if(processingThread_)
        {
            processingThread_->join();
            processingThread_.reset();
            propertyTable_["Is processing"] = "Yes";
            propertyTable_["Status"] = "Ok";
        }
        else
        {
            propertyTable_["Is processing"] = "No";
            propertyTable_["Status"] = "Critical";
        }
    }
    const uint8_t& CameraObject::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& CameraObject::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& CameraObject::getType()
    {
        return type_;
    }
    const std::string& CameraObject::getName()
    {
        return name_;
    }
    bool CameraObject::hasPropertyTable()
    {
        return !propertyTable_.empty();
    }
    bool CameraObject::insertEntry(const PropertyPair& entryPair)
    {
        return propertyTable_.insert(entryPair).second;
    }
    bool CameraObject::removeProperty(const std::string& propertyName)
    {
        auto it = propertyTable_.find(propertyName);
        
        if(it == propertyTable_.end())
        {
            return false;
        }

        propertyTable_.erase(it);

        return true;
    }
    void CameraObject::clearPropertyTable()
    {
        propertyTable_.clear();
    }
    const PropertyTable& CameraObject::getPropertyTable()
    {
        return propertyTable_;
    }
    const std::string& CameraObject::getPropertyEntryValue(const std::string& entry)
    {
        return propertyTable_[entry];
    }

    void CameraObject::doFSMProcessing()
    {
        while(true)
        {
            {
                std::lock_guard<std::mutex> lock(processingMtx_);
                if(!isProcessing_)
                {
                    break;
                }
            }
                DataPackageCPtr pkg = processor_->getPackageFromQueue(); //get the package from the camera processor.
            {
                std::lock_guard<std::mutex> lock(processingMtx_);
                if(dataDistributionInterface_)
                {
                    dataDistributionInterface_->distributeData(pkg);
                }
                else
                {
                    break;
                }
            }
        }
    }
}