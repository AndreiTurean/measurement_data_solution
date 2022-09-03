#include "CoreTestFramework.hpp"

TEST_F(CoreTestFramework, EngineShouldIntialize)
{
    ASSERT_ENGINE_INITIALIZED();
}

TEST_F(CoreTestFramework, ConfigurationManagerShouldBeVisible)
{
    ASSERT_ENGINE_INITIALIZED();
    ASSERT_MO_CREATED("libtransmitters_dummy_debug.so", MeasurementObjectType::data_source);
}
