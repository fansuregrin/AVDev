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
    int64_t duration; // in milliseconds
    int64_t nb_frames;
};

class VideoStreamInfo : public StreamInfo {
public:
    virtual std::string toStr() const;

    int width;
    int height;
    AVPixelFormat format;
};

class AudioStreamInfo : public StreamInfo {
public:
    virtual std::string toStr() const;
    std::string getFormat() const;

    int sampleRate;
    int channels;
    AVSampleFormat format;
};

std::ostream &operator<<(std::ostream &os, const StreamInfo &streamInfo);

#endif // AVDEV_STREAM_INFO_H