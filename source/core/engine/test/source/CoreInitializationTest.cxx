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
    ASSERT_MO_CREATED(TRANSMITTER_LIB_NAME);
}

TEST_F(CoreTestFramework, EngineShouldBeAbleToCreateMultipleMOs)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED(TRANSMITTER_LIB_NAME, 10);
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldAcceptOnlyUniqueMos)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_CREATE_DUPLICATE_MO(TRANSMITTER_LIB_NAME);
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldBeVisibleForReceivers)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED(RECEIVER_LIB_NAME);
}

TEST_F(CoreTestFramework, ReceiversShouldAcceptData)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED(TRANSMITTER_LIB_NAME, 5);
    ASSERT_MO_CREATED(RECEIVER_LIB_NAME);
    ASSERT_DATA_IS_PROCESSED();
}

TEST_F(CoreTestFramework, ReceiversShouldAcceptDataUnderStress)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MULTIPLE_MOS_CREATED(TRANSMITTER_LIB_NAME, 255);
    ASSERT_MO_CREATED(RECEIVER_LIB_NAME);
    ASSERT_DATA_IS_PROCESSED();
}
