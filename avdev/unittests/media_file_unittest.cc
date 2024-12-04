#if __cplusplus < 201703L
#error "C++17 or higher is required"
#endif
#include <filesystem>
#include <gtest/gtest.h>
#include <avdev/media_file.h>

namespace fs = std::filesystem;

struct GetFilenameTestCase {
    std::string input;
    bool withExt;
    std::string expectedOutput;
};

class GetFilenameTest : public testing::TestWithParam<GetFilenameTestCase> {
protected:
    void SetUp() override {
#if defined(_WIN32)
        fs::path absTempDir("C:\\avdev_temp");
        fs::create_directories(absTempDir);
#endif
        fs::path relTempDir("temp");
        fs::create_directories(relTempDir);
    }

    void TearDown() override {
        fs::remove_all("temp");
#if defined(_WIN32)
        fs::remove_all("C:\\avdev_temp");
#endif
    }

public:
    static const fs::path rootDir;
};

const fs::path GetFilenameTest::rootDir{fs::path(__FILE__).parent_path() / "../.."};

TEST_P(GetFilenameTest, GetFilename) {
    const auto& testCase = GetParam();
    fs::path inputFile(testCase.input);
    fs::create_directories(inputFile.parent_path());
    fs::copy(GetFilenameTest::rootDir / "assets/test01.mp4", inputFile,
        fs::copy_options::overwrite_existing);
    MediaFile mf(testCase.input);
    std::string filename = mf.getFilename(testCase.withExt);
    EXPECT_EQ(filename, testCase.expectedOutput);
}

INSTANTIATE_TEST_SUITE_P(
    MediaFileTests,
    GetFilenameTest,
    testing::Values(
#if defined(_WIN32)
        GetFilenameTestCase{"C:\\avdev_temp\\test01.mp4", true, "test01.mp4"},
        GetFilenameTestCase{"C:/avdev_temp/test01.mp4", true, "test01.mp4"},
#endif
        GetFilenameTestCase{"temp/test01.mp4", true, "test01.mp4"},
        GetFilenameTestCase{"temp/abc.test01.mp4", true, "abc.test01.mp4"},
        GetFilenameTestCase{"temp/ab.ef/test01", true, "test01"},
#if defined(_WIN32)
        GetFilenameTestCase{"C:\\avdev_temp\\test01.mp4", false, "test01"},
        GetFilenameTestCase{"C:/avdev_temp/test01.mp4", false, "test01"},
#endif
        GetFilenameTestCase{"temp/test01.mp4", false, "test01"},
        GetFilenameTestCase{"temp/abc.test01.mp4", false, "abc.test01"},
        GetFilenameTestCase{"temp/ab.ef/test01", false, "test01"}
    )
);