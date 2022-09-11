#pragma once
#include <iostream>
#include <gtest/gtest.h>
#include <core/Engine.hpp>

namespace core
{
    class EngineUtilis : public ::testing::Test
    {
    protected:
        std::shared_ptr<core::Engine> engine_;
    public:
        virtual void SetUp() override
        {
            engine_ = std::make_shared<core::Engine>(EngineInitFlag::performance);
            engine_->initialize();
            
        }
        const auto& getSelf()
        {
            return engine_->self_;
        }

        void getFactory()
        {
            return engine_->configMgr_->factory_->scanForMeasurementObjects(std::filesystem::current_path());
        }

    }
;}
