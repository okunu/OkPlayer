//
// Created by okunu on 2023/10/1.
//

#include "RealPlayer.h"
#include "LogUtil.h"

RealPlayer::RealPlayer() {
}

RealPlayer::~RealPlayer() {
    release();
}

int RealPlayer::player_init(const char *path) {
    video_queue.start();
    int result = 1;
    result = format_init(path);
    if (result > 0) {
        result = codec_init(AVMEDIA_TYPE_VIDEO);
    }
    return result;
}

int RealPlayer::format_init(const char *path) {
    LOGI("format_init");
    int result;
    av_register_all();
    format_context_ = avformat_alloc_context();
    result = avformat_open_input(&format_context_, path, NULL, NULL);
    if (result < 0) {
        LOGI("Player Error : Can not open video file");
        return FAIL_CODE;
    }
    result = avformat_find_stream_info(format_context_, NULL);
    if (result < 0) {
        LOGI("Player Error : Can not find video file stream info");
        return FAIL_CODE;
    }
    return SUCCESS_CODE;
}

int RealPlayer::find_stream_index(AVMediaType type) {
    LOGI("find_stream_index type = %d", type);
    int index = -1;
    for (int i = 0; i < format_context_->nb_streams; i++) {
        // 匹配视频流
        if (format_context_->streams[i]->codecpar->codec_type == type) {
            index = i;
        }
    }
    return index;
}

int RealPlayer::codec_init(AVMediaType type) {
    LOGI("codec_init type = %d", type);
    int result;
    AVFormatContext *format_context = format_context_;
    int index = find_stream_index(type);
    if (index == -1) {
        LOGI("Player Error : Can not find stream");
        return FAIL_CODE;
    }
    AVCodecContext *codec_context = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codec_context, format_context->streams[index]->codecpar);
    AVCodec *codec = avcodec_find_decoder(codec_context->codec_id);
    result = avcodec_open2(codec_context, codec, nullptr);
    if (result < 0) {
        LOGI("Player Error : Can not open codec");
        return FAIL_CODE;
    }
    if (type == AVMEDIA_TYPE_VIDEO) {
        video_stream_index = index;
        video_codec_context = codec_context;
    }
    return 1;
}

int RealPlayer::video_prepare() {
    LOGI("video_prepare");
    AVCodecContext *codec_context = video_codec_context;
    int videoWidth = codec_context->width;
    int videoHeight = codec_context->height;
    rgba_frame = av_frame_alloc();
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
    video_out_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(rgba_frame->data, rgba_frame->linesize,
                         video_out_buffer, AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
    sws_context = sws_getContext(videoWidth, videoHeight, codec_context->pix_fmt,
                                 videoWidth, videoHeight, AV_PIX_FMT_RGBA, SWS_BICUBIC,
                                 nullptr, nullptr, nullptr);
    return 1;
}

void RealPlayer::video_play(AVFrame *frame) {
    int video_height = video_codec_context->height;
    int result = sws_scale(sws_context, (const uint8_t *const *) frame->data,
                           frame->linesize, 0, video_height, rgba_frame->data,
                           rgba_frame->linesize);
    if (result <= 0) {
        LOGI("Player Error : video data convert fail");
        return;
    }
    shader_.draw(rgba_frame);
}

void RealPlayer::release() {
    LOGI("player_release");
    avformat_close_input(&(format_context_));
    avformat_free_context(format_context_);
    format_context_ = nullptr;
    avcodec_close(video_codec_context);
    avcodec_free_context(&video_codec_context);
    video_codec_context = nullptr;

    sws_freeContext(sws_context);
    av_frame_free(&(rgba_frame));
}

void RealPlayer::play_start() {
    LOGI("play_start");
    pool_.submit("produce", [&]() {
        produce();
    });
    pool_.submit("video consumer", [&](int index) {
        consumer(index);
    }, video_stream_index);
}

void RealPlayer::produce() {
    LOGI("produce");
    AVPacket *packet = av_packet_alloc();
    for (;;) {
        int result = av_read_frame(format_context_, packet);
        if (result < 0) {
            LOGI("avreadframe is less 0");
            break;
        }
        if (packet->stream_index == video_stream_index) {
            video_queue.push(packet);
        }
        packet = av_packet_alloc();
    }
    LOGI("produce video size = %d", video_queue.size());
    video_queue.stop();
    for (;;) {
        if (video_queue.is_empty()) {
            LOGI("video queue is empty and audio queue is empty");
            break;
        }
    }
    release();
    LOGI("produce end");
}

void RealPlayer::consumer(int index) {
    LOGI("consumer index = %d videoIndex = %d", index, video_stream_index);
    AVCodecContext *codec_context;
    AVStream *stream;
    BlockQueue<Element> *queue;
    int result = -1;
    if (index == video_stream_index) {
        codec_context = video_codec_context;
        stream = format_context_->streams[video_stream_index];
        queue = &video_queue;
        video_prepare();
    }
    AVFrame *frame = av_frame_alloc();
    for (;;) {
        AVPacket *packet = queue->pop();
        if (packet == nullptr) {
            LOGI("consume packet is null");
            break;
        }
        result = avcodec_send_packet(codec_context, packet);
        if (result < 0 && result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
            LOGI("Player Error : codec step 1 fail");
            av_packet_free(&packet);
            continue;
        }
        while (avcodec_receive_frame(codec_context, frame) == 0) {
            LOGI("receive frame: %d", frame->width);
            if (index == video_stream_index) {
//                video_play(frame);
            }
        }
        av_packet_unref(packet);
    }
    LOGI("consumer end index = %d video_queue.size = %d", index, video_queue.size());
    return;
}

void RealPlayer::surface_create() {
    shader_.init();
}

void RealPlayer::surface_changed(int w, int h) {
    shader_.onSurfaceChanged(w, h);
}