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
   
    Status decode(const AVPacket *packet, AVFrame *frame);
    bool isOpen() const {
        return isOpen_;
    }

protected:
    explicit Decoder(const AVCodecParameters *params);

    AVCodec *dec_;
    AVCodecContext *decCtx_;
    bool isOpen_;

private:
    bool open(const AVCodecParameters *params);
    void close();
};

class VideoDecoder : public Decoder {
public:
    explicit VideoDecoder(const AVCodecParameters *params);
};

class AudioDecoder : public Decoder {
public:
    explicit AudioDecoder(const AVCodecParameters *params);
};

#endif // AVDEV_DECODER_H