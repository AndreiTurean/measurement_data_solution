#include <core/MeasurementObjectFactory.hpp>
#include <dlfcn.h>
#include <gnu/lib-names.h>
#include <transmitters/DummyObject.hpp>
#include <iostream>

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
            void* handle = dlopen(obj.path().c_str(), RTLD_NOW);

            if(handle == nullptr)
                continue;
            auto func = dlsym(handle, "createMO");

            if(func == nullptr)
                continue;
            objectsMap_[obj.path().filename().c_str()] = func;
        }

        
    }

    std::shared_ptr<MeasurementObject> MeasurementObjectFactory::createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle)
    {
        if(name.empty())
        {
            return nullptr;
        }

        auto func = objectsMap_[name.c_str()];
        createMO_t* mo = (createMO_t*)func;

        if(!mo)
        {
            return nullptr;
        }

        return mo(nullptr, instanceNb, handle, name.c_str());
    }

    size_t MeasurementObjectFactory::getExtractedFuncSize()
    {
        return objectsMap_.size();
    }
}