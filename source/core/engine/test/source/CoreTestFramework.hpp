#pragma once
#include <core/Engine.hpp>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#ifdef _DEBUG
    #ifdef WIN32
    #define TRANSMITTER_LIB_NAME "transmitters_dummy_debug.dll"
    #define RECEIVER_LIB_NAME "processors_raw_debug.dll"
    #else
    #define TRANSMITTER_LIB_NAME "libtransmitters_dummy_debug.so"
    #define RECEIVER_LIB_NAME "libprocessors_raw_debug.so"
    #endif // WIN32
#else
    #ifdef WIN32
    #define TRANSMITTER_LIB_NAME "transmitters_dummy.dll"
    #define RECEIVER_LIB_NAME "processors_raw.dll"
    #else
    #define TRANSMITTER_LIB_NAME "libtransmitters_dummy_debug.so"
    #define RECEIVER_LIB_NAME "libprocessors_raw_debug.so"
    #endif // WIN32
#endif // DEBUG

using namespace std::chrono_literals;
class CoreTestFramework : public ::testing::Test
{
protected:
    core::Engine* engine_;
    size_t moSize_;
    
public:
    virtual void SetUp() override
    {
        moSize_ = 0;
        engine_ = new core::Engine(EngineInitFlag::no_metrics);
        ASSERT_TRUE(engine_ != nullptr);
    }
    virtual void TearDown() override
    {
        ASSERT_TRUE(engine_ != nullptr);
        engine_->terminate();
        delete engine_;
        engine_ = nullptr;
        ASSERT_TRUE(engine_ == nullptr);
    }

    /*!
    *   @brief after initialization engine should have the configuration manager ready to run
    */
    void ASSERT_ENGINE_INITIALIZED()
    {
        ASSERT_TRUE(engine_ != nullptr);
        engine_->initialize();
        ConfigurationParser* ifc = engine_->getInterface<ConfigurationParser>();
        ASSERT_TRUE(ifc != nullptr);
        ConfigurationParser* ifc2 = static_cast<ConfigurationParser*>(engine_->getInterface("ConfigurationParser"));
        EXPECT_EQ(ifc, ifc2);
    }

    void ASSERT_MO_CREATED(const std::string& name, uint8_t instancenb = 0)
    {
        ConfigurationParser* configMgr = static_cast<ConfigurationParser*>(engine_->getInterface("ConfigurationParser"));
        moSize_ = configMgr->getMOsAddedInConfig().size();
        ASSERT_TRUE(configMgr->createMeasurementObject(name, instancenb));
        auto mo = configMgr->getMOsAddedInConfig().back();

        EXPECT_EQ(mo->getName(), name);

        ASSERT_EQ(configMgr->getMOsAddedInConfig().size(), moSize_ + 1);
    }

    void ASSERT_MO_FUNC_EXTRACTED()
    {
        ConfigurationFactory* configFactory = static_cast<ConfigurationFactory*>(engine_->getInterface("ConfigurationFactory"));
        ASSERT_TRUE(configFactory != nullptr);
        EXPECT_GT(configFactory->getFactorySize(), 0);
    }

    void ASSERT_MULTIPLE_MOS_CREATED(const std::string& name, size_t count)
    {
        ConfigurationParser* conf = static_cast<ConfigurationParser*>(engine_->getInterface("ConfigurationParser"));
        moSize_ = conf->getMOsAddedInConfig().size();
        for(size_t idx = 0; idx < count; ++ idx)
        {
            std::string moName = name + std::to_string(count);
            
            ASSERT_TRUE(conf->createMeasurementObject(name, (uint8_t)idx));
            ASSERT_EQ(conf->getMOsAddedInConfig().size(), idx + 1 + moSize_);
        }
    }

    void ASSERT_CREATE_DUPLICATE_MO(const std::string& name)
    {
        bool flag = true;
        ConfigurationParser* conf = static_cast<ConfigurationParser*>(engine_->getInterface("ConfigurationParser"));
        moSize_ = conf->getMOsAddedInConfig().size();
        for(int idx = 0; idx < 10; ++idx)
        {
            ASSERT_EQ(conf->createMeasurementObject(name, 1), flag);
            for(auto mo :conf->getMOsAddedInConfig())
            {
                if(mo->getHandle() != ENGINE_HANDLE)
                    EXPECT_EQ(mo->getName(), name);
            }

            ASSERT_EQ(conf->getMOsAddedInConfig().size(), moSize_+ 1);
            flag &= false;
        }
    }
    
    void ASSERT_DATA_IS_PROCESSED()
    {
        bool skip = false;
        if(skip)
        {
            return;
        }

        int i = 0 ;
        std::this_thread::sleep_for(1s);
        while(i< 50)
        {
            std::this_thread::sleep_for(1000ms);
            DataDistributionStatistics* stat = static_cast<DataDistributionStatistics*>(engine_->getInterface("DataDistributionStatistics"));
            if (!stat)
                break;
            auto r1 = stat->getNumberOfProcessedPackagesPerSecond();
            auto r2 = stat->getMaximumProcessedPackagesPerSecond();

            std::cout<< "Current: " << r1 << ", maximum: " << r2 << std::endl;
            ASSERT_GT(r1, 3000);
            ASSERT_GT(r2, 3000);

            i++;
        }
    }

    void ASSERT_STRESS_AQUISITION()
    {
        bool skip = true;
        if(skip)
        {
            return;
        }

        int i = 0 ;
        std::this_thread::sleep_for(1s);
        while(i< 500)
        {
            std::this_thread::sleep_for(10ms);
            DataDistributionStatistics* stat = static_cast<DataDistributionStatistics*>(engine_->getInterface("DataDistributionStatistics"));
            auto r1 = stat->getNumberOfProcessedPackagesPerSecond();
            auto r2 = stat->getMaximumProcessedPackagesPerSecond();

            std::cout<< "Current: " << r1 << ", maximum: " << r2 << std::endl;
            ASSERT_GT(r1, 3000);
            ASSERT_GT(r2, 3000);

            i++;
        }
    }
};