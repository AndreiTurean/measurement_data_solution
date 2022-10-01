#pragma once
#include <cstdint>
#include <defs/MiniObjectDefs.hpp>

/*!
*   @brief Interface used to notify all the subject registered using registerToReceiverSink method to a processor
*/
struct NotifySubjects
{
    /*!
    *   @brief Method use to notify a subscriber when a data package is received.
    *   @param pkg data package that will be transmitted to the subject
    *   @return Return true if the subject was notified.
    */
    virtual bool notify(DataPackageCPtr pkg) = 0;
};

/*!
*   @brief Interface used to register a subject to a processor
*/
struct ReceiverSinkManager
{
    /*!
    *   @brief Method use to register a subject to a processor.
    *   @param subject subject that will be stored in the notification sink of the processor
    *   @return Return true if the subject was introduced to the processor sink
    */
    virtual bool registerToReceiverSink(NotifySubjects* subject) = 0;

    /*!
    *   @brief Method use to unregister a subject to a processor.
    *   @param subject subject that will be cleared from the notification sink of the processor
    *   @return Return true if the subject was cleared from the processor sink
    */
    virtual bool unregisterToReceiverSink(NotifySubjects* subject) = 0;
};