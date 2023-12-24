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
#include "AudioPlayer.h"

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
    void videoDecode(int index);

    void audioDecode();
    int audioDecodeOneFrame();
    int audio_prepare();

    void adjustVideoScale();
    void avFrameToYuvData(uint8_t* src_data[], int scr_width, int src_height, uint8_t* dst_data[], int line_size[]);
    bool handleFinish();

private:
    AVFormatContext *format_context_;

    int video_stream_index;
    AVCodecContext *video_codec_context;
    struct SwsContext *sws_context;
    BlockQueue<Element> video_queue;

    int audio_stream_index;
    AVCodecContext *audio_codec_context;
    BlockQueue<Element> audio_queue;
    struct SwrContext *swr_context;
    AudioPlayer audioPlayer;

    //采样格式，一次采样使用多少字节存储
    enum AVSampleFormat out_format_;
    //采样频率，常见的有44100
    int out_sample_rate_;
    //通道layout，声道布局，但并不是直接的声道数量
    uint64_t out_channel_layout_;
    //一帧中，单声道里有多少个采样数量
    int out_nb_samples_;
    //声道数
    int out_channels_;
    uint8_t *audio_out_buffer;

    ThreadPool pool_{3};

    VideoShader shader_;
    EglDisplay display_;
    int screen_width_;
    int screen_height_;

    uint8_t *yuv_data[4];
    int yuv_linesize[4];
    int scale_video_width;
    int scale_video_height;
    uint16_t video_width;
    uint16_t video_height;

    bool finish = false;
    double audio_clock_;
};

#endif //OKPLAYER_REALPLAYER_H
