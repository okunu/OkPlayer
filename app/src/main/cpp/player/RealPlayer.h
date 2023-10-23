//
// Created by okunu on 2023/10/1.
//

#ifndef OKPLAYER_REALPLAYER_H
#define OKPLAYER_REALPLAYER_H

#include <jni.h>

extern "C" {
#include <libavutil/avutil.h>
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}

#include "Queue.h"
#include <memory>
#include "ThreadPool.h"
#include "BlockQueue.h"
#include "VideoShader.h"
#include <unistd.h>
#include "EglDisplay.h"

#define FAIL_CODE -1
#define SUCCESS_CODE 1

#define AV_SYNC_THRESHOLD_MIN 0.04
#define AV_SYNC_THRESHOLD_MAX 0.1

using namespace std;

class RealPlayer {
public:
    typedef AVPacket* Element;
    RealPlayer();
    ~RealPlayer();
    int player_init(const char* path);
    void play_start();
    void surface_changed(int w, int h, EglDisplay& display);

private:
    int format_init(const char *path);
    int find_stream_index(AVMediaType type);
    int codec_init(AVMediaType type);
    int video_prepare();
    void video_play(AVFrame *frame);
    void release();
    void produce();
    void consumer(int index);

private:
    AVFormatContext *format_context_;

    int video_stream_index;
    AVCodecContext *video_codec_context;
    struct SwsContext *sws_context;
    uint8_t *video_out_buffer;
    AVFrame *rgba_frame;
    BlockQueue<Element> video_queue;

    ThreadPool pool_{2};

    VideoShader shader_;
    EglDisplay display_;
    int width_;
    int height_;
};

#endif //OKPLAYER_REALPLAYER_H
