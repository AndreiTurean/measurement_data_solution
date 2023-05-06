#include "pch.h"



TEST(AscLibInitTests, LibraryCanBeLoaded)
{
    core::utility::LibUtility utility(nullptr);
    void* lib = utility.openLibrary(LIB_NAME, "createReader");
    ASSERT_NE(lib, nullptr);
}

TEST(AscLibInitTests, LibraryCanBeLoadedWithValidAdress)
{
    core::utility::LibUtility utility(nullptr);
    void* lib = utility.openLibrary(LIB_NAME, "createReader");
    ASSERT_NE(lib, nullptr);
    createReader* rdr = (createReader*)lib;
    ASSERT_NE(rdr, nullptr);
    ReaderIfc* reader = rdr(nullptr);
    ASSERT_NE(reader, nullptr);
}