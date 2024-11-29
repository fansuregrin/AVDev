#include <stdexcept>
#include <avdev/media_file.h>

MediaFile::MediaFile(const std::string &filepath)
: fmtCtx_(nullptr), status_(Status::FAILURE) {
    if (avformat_open_input(&fmtCtx_, filepath.c_str(), nullptr, nullptr) == 0) {
        if (avformat_find_stream_info(fmtCtx_, nullptr) >= 0) {
            status_ = Status::SUCCESS;
        } else {
            avformat_close_input(&fmtCtx_);
        }
    }
}

MediaFile::~MediaFile() {
    if (fmtCtx_) {
        avformat_close_input(&fmtCtx_);
    }
}

int64_t MediaFile::bitRate(int streamIdx) const {
    if (streamIdx < 0 || 
        static_cast<unsigned int>(streamIdx) >= fmtCtx_->nb_streams) {
        return 0;
    }
    AVStream *st = fmtCtx_->streams[streamIdx];
    return st->codecpar->bit_rate;
}

std::unique_ptr<StreamInfo> MediaFile::getStreamInfo(int streamIdx) const {
    if (streamIdx < 0 || 
        static_cast<unsigned int>(streamIdx) >= fmtCtx_->nb_streams) {
        throw std::out_of_range("invalid stream index");
    }
    
    AVStream *st = fmtCtx_->streams[streamIdx];
    AVCodecParameters *codecpar = st->codecpar;
    
    std::unique_ptr<StreamInfo> info;
    if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        auto videoInfo = std::make_unique<VideoStreamInfo>();
        videoInfo->width = codecpar->width;
        videoInfo->height = codecpar->height;
        videoInfo->format = static_cast<AVPixelFormat>(codecpar->format);
        info = std::move(videoInfo);
    } else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
        auto audioInfo = std::make_unique<AudioStreamInfo>();
        audioInfo->sampleRate = codecpar->sample_rate;
#if (LIBAVCODEC_VERSION_MAJOR > 58)
        audioInfo->channels = codecpar->ch_layout.nb_channels;
#else
        audioInfo->channels = codecpar->channels;
#endif
        audioInfo->format = static_cast<AVSampleFormat>(codecpar->format);
        info = std::move(audioInfo);
    } else {
        info = std::make_unique<StreamInfo>();
    }

    info->codecType = codecpar->codec_type;
    info->codecId = codecpar->codec_id;
    info->bitRate = codecpar->bit_rate;
    info->duration = st->duration * st->time_base.num * 1000
        / st->time_base.den;
    info->nb_frames = st->nb_frames;

    return info;
}

std::unique_ptr<StreamInfo> MediaFile::getStreamInfo(AVMediaType type) const {
    int idx = av_find_best_stream(fmtCtx_, type, -1, -1, nullptr, 0);
    return getStreamInfo(idx);
}