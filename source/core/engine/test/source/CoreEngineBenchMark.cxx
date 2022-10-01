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

TEST_F(BenchmarkUtilis, MemoryConsumptionForDataProcessing)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
   CREATE_MO("libprocessors_raw_debug.so");
   CREATE_MO("libtransmitters_dummy_debug.so");
   ASSERT_MEM_CONSUMPTION_DATA_PROCESSING();
}

TEST_F(BenchMarkUtilisPerf, DataDistributionPerformanceEmptyPackage)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
   SMALL_PACKAGE_DISTRIBUTION();
}

TEST_F(BenchMarkUtilisPerf, DataDistributionPerformance1KBPackage)
{
   ASSERT_MEM_CONSUMPTION_ENGINE_INIT();
   SMALL_PACKAGE_DISTRIBUTION();
}