#include <core/Engine.hpp>
#include "pch.h"
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <Logger.hpp>
#include "Watchdog.hpp"

namespace core
{
    Engine::Engine(EngineInitFlag flag):
        configMgr_(nullptr),
        dataDistributionPtr_(nullptr),
        logger_(nullptr),
        watchdog_(nullptr),
        self_(nullptr),
        interfaceHelperPtr_(nullptr),
        showLogger_(false),
        showDistrMgr_(false),
        showConfigMgr_(false),
        showAbout_(false),
        showConfigurationManager_(false)
    {
        bool silentLog = false;
        bool silenceWatchDog = false;
        switch (flag)
        {
        case EngineInitFlag::normal:
            break;
        case EngineInitFlag::silent:
        {
            silentLog = true;
        }
        break;
        case EngineInitFlag::no_metrics:
        {
            silenceWatchDog = true;
        }
        break;
        case EngineInitFlag::performance:
        {
            silentLog = true;
            silenceWatchDog = true;
        }
        break;
        default:
            break;
        }
        interfaceHelperPtr_ = new core::utility::InterfaceUtilityHelper(this);
        logger_ = new Logger(this, silentLog);
        dataDistributionPtr_ = new DistributionManager(this);

        if(!silenceWatchDog)
        {
            watchdog_ = new metrics::Watchdog(logger_);
        }
    }
    Engine::~Engine()
    {
        terminate();
    }


    void* Engine::getInterface(const std::string& ifcName)
    {
        if(ifcName.find("LoggingInterface") != std::string::npos)
        {
            return dynamic_cast<LoggingInterface*>(logger_);
        }
        if (ifcName.find("DataDistributionStatistics") != std::string::npos)
        {
            return dataDistributionPtr_->getDistributionInterface();
        }
        if (ifcName.find("DistributionManagerPrivate") != std::string::npos)
        {
            return dynamic_cast<DistributionManagerPrivate*>(dataDistributionPtr_);
        }
        if(ifcName.find("DataDistribution") != std::string::npos)
        {
            return dynamic_cast<DataDistribution*>(dataDistributionPtr_);
        }
        if(ifcName.find("ConfigurationParser") != std::string::npos)
        {
            return dynamic_cast<ConfigurationParser*>(configMgr_);
        }
        if(ifcName.find("ConfigurationFactory") != std::string::npos)
        {
            return configMgr_->getInterface(ifcName);
        }

        return nullptr;
    }

    void Engine::initialize()
    {
        logger_->log("Started initialization");
        std::shared_ptr<MeasurementObjectFactory> factory = std::make_shared<MeasurementObjectFactory>(this);
        configMgr_ = new ConfigurationManager(this, factory);
        self_ = new EngineObject();

        if(!configMgr_->createMeasurementObject(self_))
        {
            logger_->log("Failed to introduce Engine Object in the configuration manager", ENGINE_HANDLE, severity::critical);
        }

        configMgr_->createMeasurementObject(watchdog_);
        logger_->log("Initialization finished");
    }
    
    void Engine::terminate()
    {
        logger_->log("Started engine termination");
        
        if(watchdog_)
        {
            logger_->log("Destroying the watchdog");
        }
        
        if (dataDistributionPtr_)
        {
            logger_->log("Stopping data distribution", ENGINE_HANDLE, severity::information);
            dataDistributionPtr_->stopDistribution();
        }

        if (configMgr_)
        {
            logger_->log("Destroying configuration manager", ENGINE_HANDLE, severity::information);
            configMgr_->terminate();
            delete configMgr_;
            configMgr_ = nullptr;
        }

        if (dataDistributionPtr_)
        {
            logger_->log("Destroying data distribution manager", ENGINE_HANDLE, severity::information);
            delete dataDistributionPtr_;
            dataDistributionPtr_ = nullptr;
        }

        logger_->log("Finished engine termination");
    }

    bool Engine::isWatchDogActive()
    {
        return watchdog_ != nullptr;
    }

    bool Engine::isLoggerActive()
    {
        return logger_ != nullptr;
    }
    bool Engine::isPerformingDataAquisition()
    {
        return dataDistributionPtr_->isDistributing();
    }

    core::utility::InterfaceUtilityHelper* Engine::getInterfaceHelper()
    {
        return interfaceHelperPtr_;
    }

    void Engine::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open configuration manager", "Ctrl+o")) { showConfigurationManager_ = true; }
                
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Show"))
            {
                //if (ImGui::MenuItem("Show configuration manager", "Ctrl+m")) { showConfigMgr_ = !showConfigMgr_; }
                if (ImGui::MenuItem("Show distribution manager", "Ctrl+d"))   { showDistrMgr_ = !showDistrMgr_; }
                if (ImGui::MenuItem("Show logger", "Ctrl+l"))  { showLogger_ = !showLogger_; }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("About"))
            {
                if (ImGui::MenuItem("About engine", "Ctrl+e")) 
                {
                    showAbout_ = true;
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        if(showConfigurationManager_)
        {
            showConfigMgr_ = false;
            showConfigurationManager_ = configMgr_->showModal(ctx);
        }

        if(showAbout_)
        {
            ImGui::OpenPopup("About engine");
            if(ImGui::BeginPopupModal("About engine", &showAbout_, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Engine version: 0.1");
                ImGui::Text("Engine build date: %s", __DATE__);
                ImGui::Text("Engine build time: %s", __TIME__);
                ImGui::Text("Engine build type: %s", "Debug");

                if (ImGui::Button("OK"))
                {
                    showAbout_ = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        
        configMgr_->show(ctx);
        if(showDistrMgr_) dataDistributionPtr_->show(ctx);
        if(showLogger_) logger_->show(ctx);
    }
}