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

        if(processingThread_)
        {
            processingThread_->join();
            processingThread_.reset();
        }
    }
    void Dummy::doFSMProcessing()
    {
        uint64_t cnt = 0;
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
                pkg->payload = new uint8_t[1024];
                pkg->size = 1024;
                pkg->timestamp = cnt++;
                dataDistributionInterface_->distributeData(pkg);
                delete[] reinterpret_cast<uint8_t*>(pkg->payload);
            }

            std::this_thread::sleep_for(1ms);
        }
    }

    void* Dummy::getInterface(const std::string&)
    {
        return nullptr;
    }
    void Dummy::startProcessing()
    {
        if(interfaceAccess_)
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            processingThread_.reset();
            dataDistributionInterface_ = reinterpret_cast<DataDistribution*>(interfaceAccess_->getInterface("DataDistribution"));
            processingThread_ = std::make_unique<std::thread>(&transmitters::Dummy::doFSMProcessing, this);
        }
    }
    void Dummy::endProcessing()
    {
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            isProcessing_ = false;
        }

        if(processingThread_)
        {
            processingThread_->join();
            processingThread_.reset();
        }
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