#pragma once
#include <statistics/MemoryUsageStatistics.hpp>
#include <core/Engine.hpp>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <malloc.h>

#pragma warning(disable: 4996)

using namespace std::chrono_literals;
class BenchmarkUtilis : public ::testing::Test
{
protected:
    std::shared_ptr<core::Engine> engine_;
    size_t initialMemory_;
    
public:
    BenchmarkUtilis():
        engine_(nullptr),
        initialMemory_(0)
    {
        initialMemory_ = core::statistics::getCurrentMemUsage();
        std::cout << "Initial memory used: "<<  initialMemory_ << " (bytes)" << std::endl;
    }
    ~BenchmarkUtilis()
    {
        size_t memLeaked = core::statistics::getCurrentMemUsage() - initialMemory_;
        std::cout << "Memory leaked during runtime: "<<  memLeaked << " (bytes)" << std::endl;
        EXPECT_LE(memLeaked, 1024 * 1024); // we expect leaks smaller than 1 MB
    }
    virtual void SetUp() override
    {
        if(engine_)
        {
            engine_->terminate();
            engine_.reset();
        }
        std::cout << "before engine creation: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
        engine_ = std::make_shared<core::Engine>(EngineInitFlag::performance);
        
        ASSERT_TRUE(engine_ != nullptr);
        std::this_thread::sleep_for(1s);
        std::cout<< "after engine creation: " << core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
    }
    virtual void TearDown() override
    {
        std::this_thread::sleep_for(1s);
        std::cout << "before engine termination: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
        ASSERT_TRUE(engine_ != nullptr);
        engine_->terminate();
        engine_.reset();
        ASSERT_TRUE(engine_ == nullptr);
        std::this_thread::sleep_for(1s);
        std::cout << "after engine termination: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
    }

    void ASSERT_MEM_CONSUMPTION_ENGINE_INIT()
    {
        std::this_thread::sleep_for(1s);
        std::cout << "before engine initialization: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
        engine_->initialize();
        std::this_thread::sleep_for(1s);
        std::cout << "after engine initialization: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
    }

    void ASSERT_MEM_CONSUMPTION_MO_CREATED(const std::string& name, uint8_t instancenb = 0)
    {
        std::this_thread::sleep_for(1s);
        std::cout << "before mo creation: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
        size_t memBefore = core::statistics::getCurrentMemUsage();
        ASSERT_TRUE(engine_->getConfigurationManager()->createMeasurementObject(name, instancenb));
        std::this_thread::sleep_for(1s);
        std::cout << "after mo creation: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
        std::cout<<"memory used to created MO: "<< core::statistics::getCurrentMemUsage() - memBefore << " (bytes)" << std::endl;
        engine_->getConfigurationManager()->clearMeasurementObjectList();
        size_t memoryWasted = core::statistics::getCurrentMemUsage() > memBefore ?core::statistics::getCurrentMemUsage() - memBefore : 0;
        std::this_thread::sleep_for(1s);
        std::cout<<"memory leaked while created MO: "<< memoryWasted << " (bytes)" << std::endl;
        std::cout << "after mo destroyed: "<< core::statistics::getCurrentMemUsage() << " (bytes)" << std::endl;
    }

    void ASSERT_MEM_CONSUMPTION_DATA_PROCESSING()
    {
        std::this_thread::sleep_for(300s);
    }

    void CREATE_MO(const std::string& name, uint8_t instancenb = 0)
    {
        ASSERT_TRUE(engine_->getConfigurationManager()->createMeasurementObject(name, instancenb));
    }
};

class BenchMarkUtilisPerf : public BenchmarkUtilis
{
public:
    virtual void SetUp() override
    {
        if(BenchmarkUtilis::engine_)
        {
            BenchmarkUtilis::engine_->terminate();
            BenchmarkUtilis::engine_.reset();
        }

        BenchmarkUtilis::engine_ = std::make_shared<core::Engine>(EngineInitFlag::performance);
    }
    virtual void TearDown() override
    {
        ASSERT_TRUE(BenchmarkUtilis::engine_ != nullptr);
        BenchmarkUtilis::engine_->terminate();
        BenchmarkUtilis::engine_.reset();
        ASSERT_TRUE(BenchmarkUtilis::engine_ == nullptr);
    }

    void DUMMY_DISTRIBUTION()
    {
        ASSERT_TRUE(BenchmarkUtilis::engine_->getConfigurationManager()->createMeasurementObject("libprocessors_raw_debug.so", 0));
        DataDistribution* dataDistributionPtr = static_cast<DataDistribution*>(BenchmarkUtilis::engine_->getInterface("DataDistribution"));
        DataPackagePtr pkg = std::make_shared<DataPackage>();
        uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        dataDistributionPtr->distributeData(pkg);

        uint64_t  deltaTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timestamp;
        std::cout << "Timestamp elapsed for distributing a package: "<< deltaTimestamp << " (us)" << std::endl;
    }

    void SMALL_PACKAGE_DISTRIBUTION()
    {
        ASSERT_TRUE(BenchmarkUtilis::engine_->getConfigurationManager()->createMeasurementObject("libprocessors_raw_debug.so", 0));
        DataDistribution* dataDistributionPtr = static_cast<DataDistribution*>(BenchmarkUtilis::engine_->getInterface("DataDistribution"));
        DataPackagePtr pkg = std::make_shared<DataPackage>();
        pkg->cycle_ = 1;
        pkg->payload = static_cast<void*>(new uint8_t[1024]);
        pkg->size = 1;
        pkg->sourceHandle = ENGINE_HANDLE;
        pkg->timestamp = 1;
        uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        dataDistributionPtr->distributeData(pkg);

        uint64_t  deltaTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timestamp;
        std::cout << "Timestamp elapsed for distributing a package: "<< deltaTimestamp << " (us)" << std::endl;
    }
};