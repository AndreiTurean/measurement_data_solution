#include <core/ConfigurationManager.hpp>
#include <XmlWrapper.hpp>
#include <algorithm>

namespace core
{
    ConfigurationManager::ConfigurationManager(InterfaceAccess* interfaceAccess, std::shared_ptr<MeasurementObjectFactory> factory):
        interfaceAccess_(interfaceAccess),
        factory_(factory)
    {

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
            return dynamic_cast<ConfigurationParser*>(this);
        if(interfaceAccess_)
            return interfaceAccess_->getInterface(interfaceName);
        return nullptr;
    }

    bool ConfigurationManager::createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle)
    {
        size_t sizeBeforeCreate = measurementObjectList_.size();
        MeasurementObjectPtr mo = factory_->createMeasurementObject(name, instanceNb, handle);
        if(std::any_of(measurementObjectList_.begin(), measurementObjectList_.end(), [&](const MeasurementObjectPtr obj)
            { 
                return obj->getHandle() == mo->getHandle();
            }))
        {
            return false;
        }
        measurementObjectList_.push_back(mo);
        return sizeBeforeCreate < measurementObjectList_.size();
    }
    bool ConfigurationManager::removeMeasurementObject(const std::string&)
    {
        return false;
    }

    const MeasurementObjectList& ConfigurationManager::getMOsAddedInConfig()
    {
        return measurementObjectList_;
    }

    size_t ConfigurationManager::getFactorySize()
    {
        return factory_->getExtractedFuncSize();
    }
}