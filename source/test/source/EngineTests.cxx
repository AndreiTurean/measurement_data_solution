#include "EngineUtilis.hpp"
using namespace core;

TEST_F(EngineUtilis, EngineShouldInitialize)
{
    engine_->initialize();
    auto self = getSelf();
    EXPECT_EQ(self->getHandle(), ENGINE_HANDLE);
    getFactory();
    engine_->terminate();
}