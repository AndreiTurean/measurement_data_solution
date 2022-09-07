#pragma once
#include <core/Engine.hpp>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
class CoreTestFramework : public ::testing::Test
{
protected:
    std::shared_ptr<core::Engine> engine_;
    size_t moSize_;
    
public:
    virtual void SetUp() override
    {
        moSize_ = 0;
        engine_ = std::make_shared<core::Engine>();
        ASSERT_TRUE(engine_ != nullptr);
    }
    virtual void TearDown() override
    {
        ASSERT_TRUE(engine_ != nullptr);
        engine_.reset();
        ASSERT_TRUE(engine_ == nullptr);
    }

    /*!
    *   @brief after initialization engine should have the configuration manager ready to run
    */
    void ASSERT_ENGINE_INITIALIZED()
    {
        ASSERT_TRUE(engine_ != nullptr);
        engine_->initialize();
        ASSERT_TRUE(engine_->getConfigurationManager() != nullptr);
    }

    void ASSERT_MO_CREATED(std::string name, uint64_t handle = 100)
    {
        auto& conf = engine_->getConfigurationManager();
        ASSERT_TRUE(conf->createMeasurementObject(name, 1, handle));
        auto mo = conf->getMOsAddedInConfig().back();

        EXPECT_EQ(mo->getName(), name);

        ASSERT_EQ(conf->getMOsAddedInConfig().size(), moSize_ + 1);
        moSize_++;
    }

    void ASSERT_MO_FUNC_EXTRACTED()
    {
        auto& conf = engine_->getConfigurationManager();
        EXPECT_GT(conf->getFactorySize(), 0);
    }

    void ASSERT_MULTIPLE_MOS_CREATED(std::string name, size_t count)
    {
        std::vector<std::string> moList;
        for(size_t idx = 0; idx < count; ++ idx)
        {
            std::string moName = name + std::to_string(count);
            auto& conf = engine_->getConfigurationManager();
            ASSERT_TRUE(conf->createMeasurementObject(name, (uint8_t)idx, idx + 100 + 5));
            
            ASSERT_EQ(conf->getMOsAddedInConfig().size(), idx + 1);
        }

        moSize_+= count;
    }

    void ASSERT_CREATE_DUPLICATE_MO(std::string name)
    {
        bool flag = true;
        for(int idx = 0; idx < 2; ++idx)
        {
            auto& conf = engine_->getConfigurationManager();
            ASSERT_EQ(conf->createMeasurementObject(name, 1, 100), flag);
            for(auto mo :conf->getMOsAddedInConfig())
            {
                EXPECT_EQ(mo->getName(), name);
            }

            ASSERT_EQ(conf->getMOsAddedInConfig().size(), 1);
            flag &= false;
        }
    }
    
    void ASSERT_DATA_IS_PROCESSED()
    {
        std::this_thread::sleep_for(120000ms);
        size_t pass = 0;
        size_t fail = 0;
        engine_->getDistributionStatistics(pass, fail);
        std::cout<<"passed pkg:" << pass << std::endl;
        std::cout<<"failed pkg:" << fail << std::endl;
        ASSERT_GT(pass, 0);
        ASSERT_EQ(fail, 0);
    }
};