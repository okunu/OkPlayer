//
// Created by okunu on 2023/9/9.
//

#ifndef DEMO_PLAYER_H
#define DEMO_PLAYER_H

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

typedef struct _Player {
    JavaVM *java_vm;
    jobject instance;
    jobject surface;

    AVFormatContext *format_context;

    int video_stream_index;
    AVCodecContext *video_codec_context;
    ANativeWindow *native_window;
    ANativeWindow_Buffer window_buffer;
    uint8_t *video_out_buffer;
    struct SwsContext *sws_context;
    AVFrame *rgba_frame;
    AVFrame *video_frame;
    Queue *video_queue;

    int audio_stream_index;
    AVCodecContext *audio_codec_context;
    uint8_t *audio_out_buffer;
    struct SwrContext *swr_context;
    int out_channel;
    jmethodID play_audio_track_method_id;
    Queue *audio_queue;

    double audio_clock;
} Player;

typedef struct _Consumer {
    Player* player;
    int stream_index;
} Consumer;

void player_init(Player **player, JNIEnv *env, jobject instance, jobject surface);

int format_init(Player* player, const char* path);

int find_stream_index(Player *player, AVMediaType type);

int codec_init(Player *player, AVMediaType type);

int video_prepare(Player *player, JNIEnv *env);

int audio_prepare(Player *player, JNIEnv* env);

void video_play(Player* player, AVFrame *frame, JNIEnv *env);

void audio_play(Player* player, AVFrame *frame, JNIEnv *env);

void player_release(Player* player);

void play_start(Player *player);

void thread_init(Player* player);

void* produce(Player* player, int* i);

void* consumer(Consumer* consumer);

#endif //DEMO_PLAYER_H
