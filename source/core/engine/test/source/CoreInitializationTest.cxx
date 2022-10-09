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
    ASSERT_MO_CREATED("transmitters_dummy_debug.dll");
}

TEST_F(CoreTestFramework, EngineShouldBeAbleToCreateMultipleMOs)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED("transmitters_dummy_debug.dll", 10);
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldAcceptOnlyUniqueMos)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_CREATE_DUPLICATE_MO("transmitters_dummy_debug.dll");
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldBeVisibleForReceivers)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED("processors_raw_debug.dll");
}

TEST_F(CoreTestFramework, ReceiversShouldAcceptData)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED("transmitters_dummy_debug.dll", 5);
    ASSERT_MO_CREATED("processors_raw_debug.dll");
    ASSERT_DATA_IS_PROCESSED();
}

TEST_F(CoreTestFramework, ReceiversShouldAcceptDataUnderStress)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED("transmitters_dummy_debug.dll", 255);
    ASSERT_MO_CREATED("processors_raw_debug.dll");
    ASSERT_DATA_IS_PROCESSED();
}
