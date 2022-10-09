#include <core/MeasurementObjectFactory.hpp>
#include <cstring>
#include <algorithm>
#include <string>


typedef MeasurementObjectPtr createMO(InterfaceAccess*, const uint8_t, const char*);

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

        std::filesystem::path curr_path = std::filesystem::path(getExePath());
        scanForMeasurementObjects(curr_path);
    }

    std::string MeasurementObjectFactory::getExePath() 
    {
#ifdef WIN32
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
#else
        return std::filesystem::current_path().string();
#endif
    }

    void MeasurementObjectFactory::scanForMeasurementObjects(std::filesystem::path path)
    {
        
        if(!std::filesystem::exists(path))
            return;

        for(auto& obj : std::filesystem::recursive_directory_iterator(path))
        {
            auto extension = obj.path().filename().extension().string();

            if(extension != ".so" && extension != ".dll")
            {
                continue;
            }

            void* func = utilityLibrary_.openLibrary(obj.path().string(), "createMO");

            if(func)
            {
                objectsMap_[obj.path().filename().u8string()] = func;
            }
        }
    }

    MeasurementObject* MeasurementObjectFactory::createMeasurementObject(const std::string& name, uint8_t instanceNb)
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

        createMO* mo = (createMO*)it->second;

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

    const FactoryMap& MeasurementObjectFactory::getFactoryMap()
    {
        return objectsMap_;
    }
}