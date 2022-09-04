#include "CoreTestFramework.hpp"

TEST_F(CoreTestFramework, EngineShouldIntialize)
{
    ASSERT_ENGINE_INITIALIZED();
}

TEST_F(CoreTestFramework, FactoryPoolShouldBeGreated)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_FUNC_EXTRACTED();
}


TEST_F(CoreTestFramework, ConfigurationManagerShouldBeVisible)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED("libtransmitters_dummy_debug.so");
}

TEST_F(CoreTestFramework, EngineShouldBeAbleToCreateMultipleMOs)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED("libtransmitters_dummy_debug.so", 10);
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldAcceptOnlyUniqueMos)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_CREATE_DUPLICATE_MO("libtransmitters_dummy_debug.so");
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldBeVisibleForReceivers)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED("libreceivers_dummy_debug.so");
}

TEST_F(CoreTestFramework, ReceiversShouldAcceptData)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED("libreceivers_dummy_debug.so");
    ASSERT_MO_CREATED("libtransmitters_dummy_debug.so", 102);
    ASSERT_DATA_IS_PROCESSED();
}
