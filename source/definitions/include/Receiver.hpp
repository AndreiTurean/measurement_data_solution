#pragma once
#include <cstdint>
#include <MiniObjectDefs.hpp>

struct NotifySubjects
{
    virtual bool notify(DataPackageCPtr pkg) = 0;
};

struct ReceiverSinkManager
{
    virtual bool registerToReceiverSink(NotifySubjects* subject) = 0;
    virtual bool unregisterToReceiverSink(NotifySubjects* subject) = 0;
};