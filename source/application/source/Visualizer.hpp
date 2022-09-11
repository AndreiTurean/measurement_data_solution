#pragma once
#include <core/ConfigurationManager.hpp>

struct Visualizer
{
    virtual void Show(std::shared_ptr<core::ConfigurationManager>&) = 0;
};

using VisualizerPtr = std::shared_ptr<Visualizer>;