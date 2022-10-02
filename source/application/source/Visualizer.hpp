#pragma once
#include <core/ConfigurationManager.hpp>

struct Visualizer
{
    virtual void Show(ConfigurationParser* ifc) = 0;
    virtual void Reset() = 0;
};

using VisualizerPtr = std::shared_ptr<Visualizer>;