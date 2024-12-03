#include <avdev/media_file.h>
#include <iostream>
#include <cassert>

int main() {
    /* test01.mp4
     * Format                                   : MPEG-4
     * Format profile                           : Base Media / Version 2
     * Codec ID                                 : mp42 (mp42/mp41/isom/avc1)
     * File size                                : 4.79 MiB
     * Duration                                 : 13 s 333 ms
     * Overall bit rate mode                    : Variable
     * Overall bit rate                         : 3 011 kb/s
     * Frame rate                               : 29.970 FPS
    */
    MediaFile mf("assets/test01.mp4");
    if (mf.getStatus() == MediaFile::Status::FAILURE) {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }

    // filename
    std::cout << "filename: " << mf.getFilename() << std::endl;

    // number of streams
    auto nbStreams = mf.nbStreams();
    std::cout << "number of streams: " << nbStreams << std::endl;
    assert(nbStreams == 2);

    // duration
    auto dur = mf.duration();
    std::cout << "duration: " << dur << " s" << std::endl;
    assert(abs(dur - 13.3333) < 1e-4);

    // overall bitrate
    auto bitrate = mf.bitRate();
    std::cout << "bitrate: " << bitrate << " bps" << std::endl;

    // format name
    std::cout << "input format name: " << mf.formatName() << std::endl;

    // streams info
    for (decltype(nbStreams) i=0; i<nbStreams; ++i) {
        std::cout << "stream " << i << " [" << *mf.getStreamInfo(i) << "]" << std::endl;
    }
}