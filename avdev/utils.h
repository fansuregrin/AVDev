#ifndef AVDEV_UTILS_H
#define AVDEV_UTILS_H

#include <cstdio>
#include <string>
#include <libavutil/avutil.h>

void printErrAndExit(int avErrno, const char *msg);

void printErr(int avErrno, const char *msg);

std::string mediaTypeToStr(const AVMediaType &type);

#endif // AVDEV_UTILS_H