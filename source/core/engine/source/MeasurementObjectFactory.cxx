#include <core/MeasurementObjectFactory.hpp>
#include <cstring>


typedef std::shared_ptr<MeasurementObject> createMO_t(InterfaceAccess*, const uint8_t, const char*);
namespace core
{
    MeasurementObjectFactory::MeasurementObjectFactory(InterfaceAccess* interfaceAccess):
        utilityLibrary_(nullptr),
        interfaceAccess_(interfaceAccess),
        logger_(nullptr)
    {
        
        logger_ = static_cast<LoggingInterface*>(interfaceAccess_->getInterface("LoggingInterface"));
        logger_->subscribe("MeasurementObjectFactory", FACTORY_HANDLE);
        utilityLibrary_ = utility::LibUtility(logger_);
        scanForMeasurementObjects(std::filesystem::current_path());
    }
    void MeasurementObjectFactory::scanForMeasurementObjects(std::filesystem::path path)
    {
        
        if(!std::filesystem::exists(path))
            return;

        for(auto& obj : std::filesystem::recursive_directory_iterator(path))
        {
            if(strcmp(obj.path().extension().c_str(), ".so") != 0)
            {
                continue;
            }

            void* func = utilityLibrary_.openLibrary(obj.path().c_str(), "createMO");

            if(func)
            {
                objectsMap_[obj.path().filename().c_str()] = func;
            }
        }
    }

    std::shared_ptr<MeasurementObject> MeasurementObjectFactory::createMeasurementObject(const std::string& name, uint8_t instanceNb)
    {
        if(name.empty())
        {
            return nullptr;
        }
        auto it = objectsMap_.find(name.c_str());

        if(it == objectsMap_.end())
        {
            return nullptr;
        }

        createMO_t* mo = (createMO_t*)it->second;

        if(!mo)
        {
            return nullptr;
        }

        return mo(interfaceAccess_, instanceNb, it->first.c_str());
    }

    size_t MeasurementObjectFactory::getFactorySize()
    {
        return objectsMap_.size();
    }

    const std::map<const std::string, void*>& MeasurementObjectFactory::getFactoryMap()
    {
        return objectsMap_;
    }
}