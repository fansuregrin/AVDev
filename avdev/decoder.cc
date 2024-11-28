#include <avdev/decoder.h>

Decoder::Decoder(const AVCodecID codecId)
: dec_(nullptr), decCtx_(nullptr) {
    dec_ = avcodec_find_decoder(codecId);
    assert(dec_ != nullptr);
}


bool Decoder::open(const AVCodecParameters *params) {
    decCtx_ = avcodec_alloc_context3(dec_);
    if (decCtx_ == nullptr) {
        return false;
    }

    int ret = avcodec_parameters_to_context(decCtx_, params);
    if (ret < 0) {
        avcodec_free_context(&decCtx_);
        return false;
    }

    ret = avcodec_open2(decCtx_, dec_, nullptr);
    if (ret < 0) {
        avcodec_free_context(&decCtx_);
        return false;
    }

    return true;
}


Decoder::Status Decoder::decode(const AVPacket *packet, AVFrame *frame) {
    int ret;
    
    ret = avcodec_send_packet(decCtx_, packet);
    if (ret < 0) {
        return Status::FAILURE;
    }
    
    ret = avcodec_receive_frame(decCtx_, frame);
    if (ret == AVERROR_EOF) {
        return Status::END;
    } else if (ret == AVERROR(EAGAIN)) {
        return Status::AGAIN;
    } else if (ret < 0) {
        return Status::FAILURE;
    }

    return Status::SUCCESS;
}

void Decoder::close() {
    if (decCtx_) {
        avcodec_free_context(&decCtx_);
        decCtx_ = nullptr;
    }
}

VideoDecoder::VideoDecoder(const AVCodecID codecId)
: Decoder(codecId) {

}

AudioDecoder::AudioDecoder(const AVCodecID codecId)
: Decoder(codecId) {

}