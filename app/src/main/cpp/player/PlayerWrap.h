//
// Created by okunu on 2023/9/23.
//

#ifndef OKPLAYER_PLAYERWRAP_H
#define OKPLAYER_PLAYERWRAP_H

#include <jni.h>

extern "C" {
#include <libavutil/avutil.h>
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "Queue.h"
#include <memory>
#include "ThreadPool.h"
#include "BlockQueue.h"

#include <unistd.h>

#define FAIL_CODE -1
#define SUCCESS_CODE 1

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

using namespace std;

class PlayerWrap {

public:
    typedef AVPacket* Element;

    PlayerWrap();

    ~PlayerWrap();

    void player_init(jobject instance, jobject surface);

    int format_init(const char *path);

    int find_stream_index(AVMediaType type);

    int codec_init(AVMediaType type);

    int video_prepare();

    int audio_prepare();

    void video_play(AVFrame *frame);

    void audio_play(AVFrame *frame);

    void release();

    void play_start();

    void *produce();

    void *consumer(int index);

private:
    jobject instance;
    jobject surface;
    jmethodID play_audio_track_method_id;

    AVFormatContext *format_context_;

    int video_stream_index;
    AVCodecContext *video_codec_context;
    ANativeWindow *native_window;
    ANativeWindow_Buffer window_buffer;
    uint8_t *video_out_buffer;
    struct SwsContext *sws_context;
    AVFrame *rgba_frame;
    BlockQueue<Element> video_queue;

    int audio_stream_index;
    AVCodecContext *audio_codec_context;
    uint8_t *audio_out_buffer;
    struct SwrContext *swr_context;
    int out_channel;
    BlockQueue<Element> audio_queue;

    double audio_clock_;

    ThreadPool pool_{3};
};

#endif //OKPLAYER_PLAYERWRAP_H
