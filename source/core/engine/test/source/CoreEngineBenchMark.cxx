#include "BenchMarkUtilis.hpp"

TEST_F(BenchmarkUtilis, MemoryConsumptionForEngineInitialization)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
}

TEST_F(BenchmarkUtilis, MemoryConsumptionForMoCreated)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
   ASSERT_MEM_CONSUMPTION_MO_CREATED("libprocessors_raw_debug.so");
}

TEST_F(BenchmarkUtilis, MemoryConsumptionForMultipleMoCreated)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
   for(uint8_t idx = 0; idx < 10; idx++)
   {
      ASSERT_MEM_CONSUMPTION_MO_CREATED("libtransmitters_dummy_debug.so", idx);
   }
}