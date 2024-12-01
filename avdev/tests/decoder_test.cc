#include <cassert>
#include <iostream>
#include <avdev/media_file.h>
#include <avdev/decoder.h>
#include <avdev/utils.h>

int main() {
    MediaFile mf("assets/test01.mp4");
    if (mf.getStatus() == MediaFile::Status::FAILURE) {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    VideoDecoder videoDecoder(mf.getAVCodecParams(AVMEDIA_TYPE_VIDEO));
    assert(videoDecoder.isOpen());

    AudioDecoder AudioDecoder(mf.getAVCodecParams(AVMEDIA_TYPE_AUDIO));
    assert(AudioDecoder.isOpen());

    std::string outDir = "output/" + mf.getFilename();
    if (!dirExists(outDir)) {
        if (!mkdir(outDir)) {
            std::cerr << "Could not make outDir: [" << outDir << "]" << std::endl;
            return 1;
        }
    }

    int videoFrameCnt = 0, audioFrameCnt = 0;
    AVFramePtr frame(av_frame_alloc());
    while (auto pkt = mf.getPacket()) {
        auto stInfo = mf.getStreamInfo(pkt->stream_index);
        auto type = stInfo->codecType;
        if (type == AVMEDIA_TYPE_VIDEO &&
            videoDecoder.decode(pkt, frame) == Decoder::Status::SUCCESS) {
            ++videoFrameCnt;
            auto videoStInfo = dynamic_cast<VideoStreamInfo *>(stInfo.get());
            auto rgbaData = VideoDecoder::convertFrameToRGBA(frame);
            char filename[64]{};
            snprintf(filename, sizeof(filename), "frame_%06d.png", videoFrameCnt);
            if (savePNG(outDir + "/" + filename, rgbaData, 
            videoStInfo->width, videoStInfo->height)) {
                std::cout << "save frame " << videoFrameCnt << " into " 
                    << filename << std::endl;
            }
        } else if (type == AVMEDIA_TYPE_AUDIO &&
            AudioDecoder.decode(pkt, frame) == Decoder::Status::SUCCESS) {
            ++audioFrameCnt;
        }
    }

    std::cout << "decoded " << videoFrameCnt << " frames of video" << std::endl;
    std::cout << "decoded " << audioFrameCnt << " frames of audio" << std::endl;
}
