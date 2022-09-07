#include <transmitters/DummyObject.hpp>
#include <chrono>

using namespace std::chrono_literals;

namespace transmitters
{
    Dummy::Dummy(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const std::string& name) :
        interfaceAccess_(interfaceAccess),
        instanceNumber_(instanceNb),
        handle_(handle),
        name_(name),
        type_(MeasurementObjectType::data_source),
        isProcessing_(true)
    {
        if(interfaceAccess_)
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            dataDistributionInterface_ = reinterpret_cast<DataDistribution*>(interfaceAccess_->getInterface("DataDistribution"));
            processingThread_ = std::make_unique<std::thread>(&transmitters::Dummy::doFSMProcessing, this);
        }
    }

    Dummy::~Dummy()
    {
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            isProcessing_ = false;
        }
        processingThread_->join();
    }
    void Dummy::doFSMProcessing()
    {
        while(true)
        {
            {
                std::lock_guard<std::mutex> lock(processingMtx_);
                if(!isProcessing_)
                {
                    break;
                }

                DataPackagePtr pkg = std::make_shared<DataPackage>(); //create a blank package
                pkg->sourceHandle = this->handle_;
                dataDistributionInterface_->distributeData(pkg);
            }

            std::this_thread::sleep_for(10ms);
        }
    }

    void* Dummy::getInterface(const std::string&)
    {
        return nullptr;
    }
    DataPackagePtr Dummy::sendPackage()
    {
        return std::make_shared<DataPackage>();
    }
    const uint8_t& Dummy::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& Dummy::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& Dummy::getType()
    {
        return type_;
    }
    const std::string& Dummy::getName()
    {
        return name_;
    }
}

std::shared_ptr<MeasurementObject> createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const char* name)
{
	return std::make_shared<transmitters::Dummy>(interfaceAccess, instanceNb, handle, name);
}