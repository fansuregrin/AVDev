#ifndef AVDEV_DECODER_H
#define AVDEV_DECODER_H

#include <cassert>
extern "C" {
#include <libavcodec/avcodec.h>
}

class Decoder {
public:
    enum Status {
        SUCCESS,
        FAILURE,
        AGAIN,
        END
    };

    virtual ~Decoder() {
        close();
    }
   
    bool open(const AVCodecParameters *params);
    Status decode(const AVPacket *packet, AVFrame *frame);
    void close();

protected:
    explicit Decoder(const AVCodecID codecId);

    AVCodec *dec_;
    AVCodecContext *decCtx_;
};

class VideoDecoder : public Decoder {
public:
    explicit VideoDecoder(const AVCodecID codecId);
};

class AudioDecoder : public Decoder {
public:
    explicit AudioDecoder(const AVCodecID codecId);
};

#endif // AVDEV_DECODER_H