#ifndef AVDEV_PTR_H
#define AVDEV_PTR_H

#include <memory>
extern "C" {
#include <libavcodec/avcodec.h>
}

struct AVPacketDeleter {
    void operator()(AVPacket *pkt) const {
        av_packet_free(&pkt);
    }
};

using AVPacketPtr = std::unique_ptr<AVPacket, AVPacketDeleter>;

struct AVFrameDeleter {
    void operator()(AVFrame *frame) const {
        av_frame_free(&frame);
    }
};

using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;

#endif // AVDEV_PTR_H