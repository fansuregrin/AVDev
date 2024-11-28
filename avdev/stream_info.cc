#include <sstream>
#include <avdev/stream_info.h>

std::string StreamInfo::toStr() const {
    std::ostringstream oss;
    oss
        << "codec name: " << avcodec_get_name(codecId)
        << ", codec type: " << av_get_media_type_string(codecType)
        << ", bit rate: " << bitRate << " bps"
    ;
    return oss.str();
}

std::string VideoStreamInfo::toStr() const {
    std::ostringstream oss;
    oss
        << ", " << width << "x" << height;
    ;
    return StreamInfo::toStr() + oss.str();
}

std::string AudioStreamInfo::toStr() const {
    std::ostringstream oss;
    oss
        << ", " << "sampel rate: " << sampleRate
        << ", " << "channels: " << channels
    ;
    return StreamInfo::toStr() + oss.str();
}

std::ostream &operator<<(std::ostream &os, const StreamInfo &streamInfo) {
    return os << streamInfo.toStr();
}