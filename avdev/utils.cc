#if __cplusplus >= 201703L
#include <filesystem>
#elif defined(_WIN32)
#include <Windows.h>
#include <direct.h>
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#include <sys/stat.h>
#endif
#include <png.h>
#include <avdev/utils.h>

void printErrAndExit(int avErrno, const char *msg) {
    char errBuf[128]{};
    av_strerror(avErrno, errBuf, sizeof(errBuf));
    fprintf(stderr, "%s: %s\n", msg, errBuf);
    exit(1);
}

void printErr(int avErrno, const char *msg) {
    char errBuf[128]{};
    av_strerror(avErrno, errBuf, sizeof(errBuf));
    fprintf(stderr, "%s: %s\n", msg, errBuf);
}

std::string mediaTypeToStr(const AVMediaType &type) {
    return av_get_media_type_string(type);
}

bool savePNG(const std::string &filename, const std::vector<uint8_t> &rgbaData,
int width, int height) {
    auto fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr) {
        return false;
    }

    // initialize png struct
    auto pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
        nullptr, nullptr, nullptr);
    if (pngPtr == nullptr) {
        fclose(fp);
        return false;
    }

    // initialize png info struct
    auto pngInfoPtr = png_create_info_struct(pngPtr);
    if (pngInfoPtr == nullptr) {
        png_destroy_write_struct(&pngPtr, nullptr);
        fclose(fp);
        return false;
    }

    // handle error
    if (setjmp(png_jmpbuf(pngPtr))) {
        png_destroy_write_struct(&pngPtr, &pngInfoPtr);
        fclose(fp);
        return false;
    }

    // initialize io
    png_init_io(pngPtr, fp);
    // 
    png_set_IHDR(pngPtr, pngInfoPtr, width, height, 8, PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    // 
    png_write_info(pngPtr, pngInfoPtr);
    int rowBytes = width * 4;
    for (int y=0; y<height; ++y) {
        png_write_row(pngPtr,
            const_cast<png_const_bytep>(rgbaData.data() + y * rowBytes));
    }
    // 
    png_write_end(pngPtr, pngInfoPtr);

    png_destroy_write_struct(&pngPtr, &pngInfoPtr);
    fclose(fp);
    return true;
}

bool dirExists(const std::string &dir) {
#if __cplusplus >= 201703L
    namespace fs = std::filesystem;
    fs::path dirPath = dir;
    return fs::exists(dirPath) && fs::is_directory(dirPath);
#elif defined(_WIN32)
    DWORD attr = GetFileAttributesA(dir.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR);
#endif
}

bool mkdir(const std::string &dir) {
#if __cplusplus >= 201703L
    std::filesystem::path dirPath = dir;
    return std::filesystem::create_directories(dirPath);
#elif defined(_WIN32)
    return (_mkdir(dir.c_str()) == 0 ? true : false);
#elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
    return (mkdir(dir.c_str(), 0755) == 0 ? true : false);
#endif
}