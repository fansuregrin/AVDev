#ifndef AVDEV_UTILS_H
#define AVDEV_UTILS_H

#include <cstdio>
#include <string>
#include <vector>
extern "C" {
#include <libavutil/avutil.h>
}

void printErrAndExit(int avErrno, const char *msg);

void printErr(int avErrno, const char *msg);

std::string mediaTypeToStr(const AVMediaType &type);

bool savePNG(const std::string &filename, const std::vector<uint8_t> &rgbaData,
int width, int height);

bool dirExists(const std::string &dir);

bool mkdir(const std::string &dir);

#endif // AVDEV_UTILS_H