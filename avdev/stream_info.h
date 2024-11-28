#ifndef AVDEV_STREAM_INFO_H
#define AVDEV_STREAM_INFO_H

#include <string>
#include <iostream>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

class StreamInfo {
public:
    virtual ~StreamInfo() = default;
    virtual std::string toStr() const;

    AVMediaType codecType;
    AVCodecID codecId;
    int64_t bitRate;
};

class VideoStreamInfo : public StreamInfo {
public:
    virtual std::string toStr() const;

    int width;
    int height;
};

class AudioStreamInfo : public StreamInfo {
public:
    virtual std::string toStr() const;

    int sampleRate;
    int channels;
};

std::ostream &operator<<(std::ostream &os, const StreamInfo &streamInfo);

#endif // AVDEV_STREAM_INFO_H