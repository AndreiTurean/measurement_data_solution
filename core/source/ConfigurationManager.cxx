#include <core/ConfigurationManager.hpp>
#include <XmlWrapper.hpp>

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

    void* ConfigurationManager::getInterface(std::string_view interfaceName)
    {
        if(interfaceName == "ConfigurationParser")
            return dynamic_cast<ConfigurationParser*>(this);
        if(interfaceAccess_)
            return interfaceAccess_->getInterface(interfaceName);
        return nullptr;
    }

    bool ConfigurationManager::createMeasurementObject(std::string_view name, uint8_t instanceNb, uint64_t handle, MeasurementObjectType type)
    {
        size_t sizeBeforeCreate = measurementObjectList_.size();
        //measurementObjectList_.push_back();
        auto res = factory_->createMeasurementObject(name, instanceNb, handle, type);

        return sizeBeforeCreate < measurementObjectList_.size();
    }
    bool ConfigurationManager::removeMeasurementObject(std::string_view)
    {
        return false;
    }

    const MeasurementObjectList& ConfigurationManager::getMOsAddedInConfig()
    {
        return measurementObjectList_;
    }
}