#include "pch.h"
#include "SignalExtraction.hpp"
#include <string>

core::signal::SignalExtraction::SignalExtraction(InterfaceAccess* interfaceAccess):
    interfaceAccess_(interfaceAccess)
{

}

void* core::signal::SignalExtraction::getInterface(const std::string& interfaceName)
{
    if (interfaceName == "SignalExtractionInterface")
    {
        return dynamic_cast<SignalExtractionInterface*>(this);
    }
    return interfaceAccess_->getInterface(interfaceName);
}

bool core::signal::SignalExtraction::extract(DataPackageCPtr ptr, SignalGroup& outSignalMap)
{
    switch(ptr->type)
    {
    case PackageType::c_struct: return extract(reinterpret_cast<SineWave*>(ptr->payload), ptr->sourceHandle, outSignalMap);
    }
    return false;
}

bool core::signal::SignalExtraction::extract(SineWave* data, uint64_t handle, SignalGroup& outSignalMap)
{
    SignalMessage msg;
    msg["Frequency"] = std::to_string(data->getFrequency());
    msg["Amplitude"] = std::to_string(data->getAmplitude());
    msg["Value"] = std::to_string(data->getValue());
    msg["Phase"] = std::to_string(data->getPhase());
    outSignalMap[std::to_string(handle)] = msg;
    return true;
}