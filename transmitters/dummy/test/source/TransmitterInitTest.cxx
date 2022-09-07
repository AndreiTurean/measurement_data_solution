#include "TransmitterTestFramework.hpp"

TEST_F(TransmitterTestFramework, ShouldIntialize)
{
    ASSERT_OBJECT_CREATED();
}

TEST_F(TransmitterTestFramework, ShouldResetWithNewHandle)
{
    ASSERT_OBJECT_HANDLE(1234567);
}

TEST_F(TransmitterTestFramework, ShouldResetWithNewInstanceNumber)
{
    ASSERT_OBJECT_INSTANCE_NB(5);
}

TEST_F(TransmitterTestFramework, ShouldResetWithNewName)
{
    ASSERT_OBJECT_NAME("important_object");
}