#include <core/MeasurementObjectFactory.hpp>
#include <utilis/LibUtility.hpp>

typedef std::shared_ptr<MeasurementObject> createMO_t(InterfaceAccess*, const uint8_t, uint64_t, const char*);
namespace core
{
    MeasurementObjectFactory::MeasurementObjectFactory()
    {

    }
    
    void MeasurementObjectFactory::scanForMeasurementObjects(std::filesystem::path path)
    {
        
        if(!std::filesystem::exists(path))
            return;

        for(auto& obj : std::filesystem::recursive_directory_iterator(path))
        {
            void* func = core::utility::LibUtility::openLibrary(obj.path().c_str(), "createMO");

            if(func)
            {
                objectsMap_[obj.path().filename().c_str()] = func;
            }
        }
    }

    std::shared_ptr<MeasurementObject> MeasurementObjectFactory::createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle)
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

        return mo(nullptr, instanceNb, handle, it->first.c_str());
    }

    size_t MeasurementObjectFactory::getExtractedFuncSize()
    {
        return objectsMap_.size();
    }
}