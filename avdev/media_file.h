#ifndef AVDEV_MEDIA_FILE_H
#define AVDEV_MEDIA_FILE_H

#include <string>
#include <memory>
#include <avdev/stream_info.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
}

class MediaFile {
public:
    enum class Status {
        SUCCESS,
        FAILURE
    };

    MediaFile(const std::string &filepath);
    ~MediaFile();

    Status getStatus() const {
        return status_;
    }

    unsigned int nbStreams() {
        return fmtCtx_->nb_streams;
    }

    /**
     * @brief Get the duration of the media file in seconds.
     * 
     * @return `double` Duration of the media file in seconds.
     */
    double duration() {
        return static_cast<double>(fmtCtx_->duration) / AV_TIME_BASE;
    }

    /**
     * @brief Get the total stream bitrate in bit/s.
     * 
     * @return `int64_t` 0 if not available
     */
    int64_t bitRate() const {
        return fmtCtx_->bit_rate;
    }

    int64_t bitRate(unsigned int streamIdx) const;

    const AVCodecParameters *getAVCodecParams(unsigned int streamIdx) const;

    const AVCodecParameters *getAVCodecParams(AVMediaType type) const;

    std::unique_ptr<StreamInfo> getStreamInfo(unsigned int streamIdx) const;

    std::unique_ptr<StreamInfo> getStreamInfo(AVMediaType type) const;

    std::string formatName() const {
        return fmtCtx_->iformat->name;
    }

private:
    AVFormatContext *fmtCtx_;
    Status status_;
};

#endif // AVDEV_MEDIA_FILE_H