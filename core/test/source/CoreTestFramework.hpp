#pragma once
#include <core/Engine.hpp>
#include <gtest/gtest.h>

class CoreTestFramework : public ::testing::Test
{
protected:
    std::shared_ptr<core::Engine> engine_;
    
public:
    virtual void SetUp() override
    {
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

    void ASSERT_MO_CREATED(std::string name)
    {
        auto& conf = engine_->getConfigurationManager();
        ASSERT_TRUE(conf->createMeasurementObject(name, 1, 100));
        for(auto mo :conf->getMOsAddedInConfig())
        {
            EXPECT_EQ(mo->getName(), name);
        }

        ASSERT_EQ(conf->getMOsAddedInConfig().size(), 1);
    }

    void ASSERT_MO_FUNC_EXTRACTED()
    {
        auto& conf = engine_->getConfigurationManager();
        EXPECT_GT(conf->getFactorySize(), 0);
    }
};