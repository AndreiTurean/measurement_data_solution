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
            void* handle = dlopen(obj.path().c_str(), RTLD_LAZY);

            if(handle == nullptr)
                continue;
            auto func = dlsym(handle, "createMO");

            if(func == nullptr)
                continue;
            std::cout<<obj.path().filename().c_str()<< std::endl;
            objectsMap_["CAN"] = func;
        }

        
    }
    const std::vector<std::string_view>& MeasurementObjectFactory::getMeasurementObjectList()
    {
        return binaryList_;
    }
    std::shared_ptr<MeasurementObject> MeasurementObjectFactory::createMeasurementObject(std::string_view name, uint8_t instanceNb, uint64_t handle, MeasurementObjectType type)
    {
        if(name.empty())
        {
            return nullptr;
        }

        switch (type)
        {
        case MeasurementObjectType::data_receiver:
        {
            auto func = objectsMap_[name];
            createMO_t* mo = (createMO_t*)func;

            return mo(nullptr, instanceNb, handle, "nume");
        }
        case MeasurementObjectType::data_source:
        {
            auto func = objectsMap_[name];
            createMO_t* mo = (createMO_t*)func;

            auto mos = mo(nullptr, instanceNb, handle, "nume");

            return mos;
        }
        case MeasurementObjectType::player:
        {
            return nullptr;
        }
        case MeasurementObjectType::recorder:
        {
            return nullptr;
        }
        default: return nullptr;
        }
        return nullptr;
    }
}