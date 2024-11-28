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