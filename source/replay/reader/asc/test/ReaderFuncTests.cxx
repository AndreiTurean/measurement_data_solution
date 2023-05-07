#include "pch.h"

class ReaderFuncTests : public ::testing::Test
{
protected:
    ReaderIfc* reader = nullptr;
    core::Engine* engine;
    std::string libPath;

public:
    void SetUp() override
    {
        engine = new core::Engine(EngineInitFlag::performance);
        engine->initialize();

        core::utility::LibUtility utility(reinterpret_cast<LoggingInterface*>(engine->getInterface("LoggingInterface")));
#if defined(_WIN32)
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring::size_type pos = std::string(buffer).find_last_of("\\/");
        libPath = std::string(buffer).substr(0, pos) + "/" + LIB_NAME;
#else
        libPath = std::filesystem::current_path().string() + "/" + LIB_NAME;
#endif
        void* lib = utility.openLibrary(libPath, "createReader");
        
        ASSERT_NE(lib, engine);
        createReader* rdr = (createReader*)lib;
        ASSERT_NE(rdr, nullptr);
        reader = rdr(engine);
        ASSERT_NE(reader, nullptr);
        ASSERT_EQ(reader->getExtension(), ".asc");
    }
    void TearDown() override
    {
        engine->terminate();
        delete engine;
        reader->closeFile();
        ASSERT_NE(reader, nullptr);
        reader = nullptr;
        ASSERT_EQ(reader, nullptr);
    }

    void loadFile(const char* fileName)
    {
        ASSERT_NE(reader, nullptr);
        ASSERT_TRUE(reader->openFile(fileName));
    }
};

TEST_F(ReaderFuncTests, ReaderCanReadFile)
{
    ASSERT_NE(reader, nullptr);
    ASSERT_TRUE(reader->openFile("test.asc"));
}

TEST_F(ReaderFuncTests, ReaderCanRetreivePosition)
{
    loadFile("test.asc");
    EXPECT_EQ(reader->getCurrentPosition(), 0);
    EXPECT_GT(reader->getEndPosition(), 0);
}

TEST_F(ReaderFuncTests, ReaderCanReadDataPackage)
{
    loadFile("test.asc");
    DataPackageCPtr data = reader->readData();
    
    ASSERT_NE(data, nullptr);

    EXPECT_EQ(data->cycle_, 15);
    EXPECT_EQ(data->size, 10);
    EXPECT_TRUE(std::memcmp(data->payload, "0000FAFA0000FAFA0080", data->size) == 0);
    EXPECT_EQ(data->sourceHandle, 0XDEAD);
    EXPECT_EQ(data->timestamp, 16696363327601);
    EXPECT_EQ(data->type, PackageType::can);
}