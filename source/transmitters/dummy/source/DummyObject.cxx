#include <transmitters/DummyObject.hpp>
#include <chrono>

using namespace std::chrono_literals;

namespace transmitters
{
    Dummy::Dummy(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name) :
        interfaceAccess_(interfaceAccess),
        instanceNumber_(instanceNb),
        handle_(INVALID_HANDLE),
        name_(name),
        type_(MeasurementObjectType::data_source),
        isProcessing_(true)
    {
        handle_ = (instanceNb + 1);
        handle_ = handle_ << 0x8 << 0x8;

        if(interfaceAccess_)
        {
            std::lock_guard<std::mutex> lock(processingMtx_);
            dataDistributionInterface_ = static_cast<DataDistribution*>(interfaceAccess_->getInterface("DataDistribution"));
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
        dataDistributionInterface_ = nullptr;
    }
    void Dummy::doFSMProcessing()
    {
        uint64_t cnt = 0;
        while(true)
        {
            DataPackagePtr pkg = new DataPackage(); //create a blank package
            pkg->sourceHandle = this->handle_;
            pkg->size = 1024;
            pkg->timestamp = cnt++;
            {
                std::lock_guard<std::mutex> lock(processingMtx_);
                
                if(!isProcessing_)
                {
                    break;
                }
                dataDistributionInterface_->distributeData(pkg);
                
            }
            delete pkg;
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
            delete this;
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