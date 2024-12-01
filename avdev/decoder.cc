#include <avdev/decoder.h>

Decoder::Decoder(const AVCodecParameters *params)
: dec_(nullptr), decCtx_(nullptr) {
    isOpen_ = open(params) ? true : false;
}


bool Decoder::open(const AVCodecParameters *params) {
    if (params == nullptr) {
        return false;
    }

    dec_ = avcodec_find_decoder(params->codec_id);
    if (dec_ == nullptr) {
        return false;
    }

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


Decoder::Status Decoder::decode(const AVPacketPtr &packet, const AVFramePtr &frame) {
    if (!isOpen_) return Status::FAILURE;

    int ret;
    
    ret = avcodec_send_packet(decCtx_, packet.get());
    if (ret < 0) {
        return Status::FAILURE;
    }
    
    ret = avcodec_receive_frame(decCtx_, frame.get());
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
    isOpen_ = false;
}

VideoDecoder::VideoDecoder(const AVCodecParameters *params)
: Decoder(params) {

}

std::vector<uint8_t> VideoDecoder::convertFrameToRGBA(const AVFramePtr &frame) {
    if (frame == nullptr) return std::vector<uint8_t>();
    
    int width = frame->width, height = frame->height;
    SwsContext *ctx = sws_getContext(
        width, height, static_cast<AVPixelFormat>(frame->format),
        width, height, AV_PIX_FMT_RGBA, 0, nullptr, nullptr, nullptr
    );
    if (ctx == nullptr) {
        return std::vector<uint8_t>();
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
    std::vector<uint8_t> buffer(numBytes, 0);
    uint8_t * dstData[AV_NUM_DATA_POINTERS] = {buffer.data()};
    int dstLinesize[AV_NUM_DATA_POINTERS] = {width * 4};
    sws_scale(ctx, frame->data, frame->linesize, 0, height, dstData, dstLinesize);
    sws_freeContext(ctx);

    return buffer;
}

AudioDecoder::AudioDecoder(const AVCodecParameters *params)
: Decoder(params) {

}