#pragma once
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <transmitters/DummyObject.hpp>
using namespace std::chrono_literals;
class TransmitterTestFramework : public ::testing::Test
{
protected:
    std::shared_ptr<transmitters::Dummy> object_;
public:
    virtual void SetUp() override
    {
       object_ = std::make_shared<transmitters::Dummy>(nullptr, 0, 10, "dummy");
    }
    virtual void TearDown() override
    {
        
    }

    void ASSERT_OBJECT_CREATED()
    {
        ASSERT_TRUE(object_ != nullptr);
        ASSERT_EQ(object_->getType(), MeasurementObjectType::data_source);
    }

    void ASSERT_OBJECT_HANDLE(uint64_t handle)
    {
        object_.reset();
        object_ = std::make_shared<transmitters::Dummy>(nullptr, 0, handle, "dummy");
        EXPECT_EQ(object_->getHandle(), handle);
    }

    void ASSERT_OBJECT_INSTANCE_NB(uint8_t instanceNumber)
    {
        object_.reset();
        object_ = std::make_shared<transmitters::Dummy>(nullptr, instanceNumber, 100, "dummy");
        EXPECT_EQ(object_->getInstanceNumber(), instanceNumber);
    }

    void ASSERT_OBJECT_NAME(std::string name)
    {
        object_.reset();
        object_ = std::make_shared<transmitters::Dummy>(nullptr, 0, 100, name);
        EXPECT_EQ(object_->getName(), name);
    }
};