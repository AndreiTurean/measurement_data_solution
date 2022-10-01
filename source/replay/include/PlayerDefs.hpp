#pragma once
#include <cstdint>
#include <defs/MiniObjectDefs.hpp>

enum class Direction
{
    forward,
    backward,
};

struct PlayerMovement
{
    virtual bool move(const uint64_t steps = 1, Direction direction = Direction::forward) = 0;
};

struct PlayerInterface
{
    virtual bool play() = 0;
    virtual bool pause() = 0;
};

struct PlayerPosition
{
    virtual uint64_t getPosition() = 0;
    virtual bool setPosition(const uint64_t& position) = 0;
};

struct PlayerBuffer
{
    virtual bool addPackage(const DataPackage package) = 0;
    virtual size_t getBufferSize() = 0;
    virtual void clearBuffer()= 0;
};