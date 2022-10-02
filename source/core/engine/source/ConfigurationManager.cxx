#include <core/ConfigurationManager.hpp>
#include <XmlWrapper.hpp>
#include <algorithm>
#include <core/DistributionManager.hpp>
#include <defs/Receiver.hpp>

namespace core
{
    ConfigurationManager::ConfigurationManager(InterfaceAccess* interfaceAccess, std::shared_ptr<MeasurementObjectFactory> factory):
        interfaceAccess_(interfaceAccess),
        factory_(factory)
    {
        logger_ = static_cast<LoggingInterface*>(interfaceAccess_->getInterface("LoggingInterface"));
        logger_->subscribe("ConfigurationManager", CONFIGURATION_MGR_HANDLE);
    }
    const MeasurementObjectList& ConfigurationManager::loadConfiguration(std::filesystem::path path)
    {
        if(std::filesystem::exists(path))
            return measurementObjectList_;

        measurementObjectList_.clear();

        //! @todo parse the XML file and repoputlate the object list use the xml wrapper
        //utility::parser::XmlWrapper xml;

        return measurementObjectList_;
    }

    void* ConfigurationManager::getInterface(const std::string& interfaceName)
    {
        if(interfaceName == "ConfigurationParser")
        {
            return dynamic_cast<ConfigurationParser*>(this);
        }
        if(interfaceName == "ConfigurationFactory")
        {
            return std::dynamic_pointer_cast<ConfigurationFactory>(factory_).get();
        }
        if(interfaceAccess_)
        {
            return interfaceAccess_->getInterface(interfaceName);
        }
        return nullptr;
    }

    bool ConfigurationManager::createMeasurementObject(const std::string& name, uint8_t instanceNb)
    {
        std::string msg = "Started creating object: " + name + " with instance number: " + std::to_string((int) instanceNb);
        logger_->log(msg.c_str(), 1);
        size_t sizeBeforeCreate = measurementObjectList_.size();
        MeasurementObjectPtr mo = factory_->createMeasurementObject(name, instanceNb);
        if(std::any_of(measurementObjectList_.begin(), measurementObjectList_.end(), [&](const MeasurementObjectPtr obj)
            { 
                return obj->getHandle() == mo->getHandle();
            }))
        {
            logger_->log("Failed to create duplicate measurement object", 1, severity::error);
            return false;
        }

        if(mo->getType() == MeasurementObjectType::data_receiver)
        {
            if(std::any_of(measurementObjectList_.begin(), measurementObjectList_.end(), [&](const MeasurementObjectPtr obj)
            { 
                return obj->getName() == mo->getName();
            }))
            {
                logger_->log("There is already a identical processor present in the configuration manager", 1, severity::error);
                return false;
            }
            auto ifc = static_cast<DistributionManagerPrivate*>(interfaceAccess_->getInterface("DistributionManagerPrivate"));
            ifc->addReceiver(std::dynamic_pointer_cast<DataReceiverObject>(mo));
        }

        measurementObjectList_.push_back(mo);

        std::string msg2 = "Finished creating object: " + name + " with instance number: " + std::to_string((int) instanceNb);
        logger_->log(msg2.c_str(), 1);
        return sizeBeforeCreate < measurementObjectList_.size();
    }
    bool ConfigurationManager::removeMeasurementObject(const std::string& name)
    {
        auto it = std::remove_if(measurementObjectList_.begin(), measurementObjectList_.end(), [&]
        (auto obj)
        {
            return obj->getName() == name;
        });

        if(it == measurementObjectList_.end())
        {
            return false;
        }
        measurementObjectList_.erase(it, measurementObjectList_.end());

        return true;
    }

    const MeasurementObjectList& ConfigurationManager::getMOsAddedInConfig()
    {
        return measurementObjectList_;
    }

    bool ConfigurationManager::createMeasurementObject(MeasurementObjectPtr object)
    {
        if(std::any_of(measurementObjectList_.begin(), measurementObjectList_.end(), [&](const MeasurementObjectPtr obj)
            { 
                return obj->getHandle() == object->getHandle();
            }))
        {
            logger_->log("Failed to create duplicate measurement object", 1, severity::error);
            return false;
        }

        measurementObjectList_.push_back(object);
        return true;
    }

    void ConfigurationManager::terminate()
    {
        measurementObjectList_.clear();
        factory_.reset();
    }

    void ConfigurationManager::clearMeasurementObjectList()
    {
        measurementObjectList_.clear();
    }
}