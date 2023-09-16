//
// Created by okunu on 2023/9/9.
//
#include "Player.h"
#include "LogUtil.h"
#include "JniHelper.h"
#include <unistd.h>

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

#define FAIL_CODE -1

void player_init(Player **player, JNIEnv *env, jobject instance, jobject surface) {
    *player = (Player *) malloc(sizeof(Player));
    JavaVM *java_vm;
    env->GetJavaVM(&java_vm);
    (*player)->java_vm = java_vm;
    (*player)->instance = env->NewGlobalRef(instance);
    (*player)->surface = env->NewGlobalRef(surface);
}

int format_init(Player *player, const char *path) {
    int result;
    av_register_all();
    player->format_context = avformat_alloc_context();
    result = avformat_open_input(&(player->format_context), path, NULL, NULL);
    if (result < 0) {
        LOGI("Player Error : Can not open video file");
        return FAIL_CODE;
    }
    result = avformat_find_stream_info(player->format_context, NULL);
    if (result < 0) {
        LOGI("Player Error : Can not find video file stream info");
        return FAIL_CODE;
    }
    return 1;
}

int find_stream_index(Player *player, AVMediaType type) {
    int index = -1;
    for (int i = 0; i < player->format_context->nb_streams; i++) {
        // 匹配视频流
        if (player->format_context->streams[i]->codecpar->codec_type == type) {
            index = i;
        }
    }
    return index;
}

int codec_init(Player *player, AVMediaType type) {
    int result;
    AVFormatContext *format_context = player->format_context;
    int index = find_stream_index(player, type);
    if (index == -1) {
        LOGI("Player Error : Can not find stream");
        return -1;
    }
    AVCodecContext *codec_context = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(codec_context, format_context->streams[index]->codecpar);
    AVCodec *codec = avcodec_find_decoder(codec_context->codec_id);
    result = avcodec_open2(codec_context, codec, nullptr);
    if (result < 0) {
        LOGI("Player Error : Can not open codec");
        return FAIL_CODE;
    }
    if (type == AVMEDIA_TYPE_AUDIO) {
        player->audio_stream_index = index;
        player->audio_codec_context = codec_context;
    } else if (type == AVMEDIA_TYPE_VIDEO) {
        player->video_stream_index = index;
        player->video_codec_context = codec_context;
    }
    return 1;
}

int video_prepare(Player *player, JNIEnv *env) {
    AVCodecContext *codec_context = player->video_codec_context;
    int videoWidth = codec_context->width;
    int videoHeight = codec_context->height;
    player->native_window = ANativeWindow_fromSurface(env, player->surface);
    if (player->native_window == NULL) {
        LOGI("Player Error : Can not create native window");
        return FAIL_CODE;
    }
    int result = ANativeWindow_setBuffersGeometry(player->native_window, videoWidth, videoHeight,
                                                  WINDOW_FORMAT_RGBA_8888);
    if (result < 0) {
        LOGI("Player Error : Can not set native window buffer");
        ANativeWindow_release(player->native_window);
        return FAIL_CODE;
    }
    player->rgba_frame = av_frame_alloc();
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
    player->video_out_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(player->rgba_frame->data, player->rgba_frame->linesize,
                         player->video_out_buffer, AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
    player->sws_context = sws_getContext(videoWidth, videoHeight, codec_context->pix_fmt,
                                         videoWidth, videoHeight, AV_PIX_FMT_RGBA, SWS_BICUBIC,
                                         nullptr, nullptr, nullptr);
    return 1;
}

int audio_prepare(Player *player, JNIEnv *env) {
    AVCodecContext *codec_context = player->audio_codec_context;
    player->swr_context = swr_alloc();
    player->audio_out_buffer = (uint8_t *) av_malloc(44100 * 2);
    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_format = AV_SAMPLE_FMT_S16;
    int out_sample_rate = player->audio_codec_context->sample_rate;
    swr_alloc_set_opts(player->swr_context,
                       out_channel_layout, out_format, out_sample_rate,
                       codec_context->channel_layout, codec_context->sample_fmt,
                       codec_context->sample_rate,
                       0, NULL);
    swr_init(player->swr_context);
    player->out_channel = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    jclass player_class = env->GetObjectClass(player->instance);
    jmethodID create_audio_track_method_id = env->GetMethodID(player_class, "createAudioTrack",
                                                              "(II)V");
    env->CallVoidMethod(player->instance, create_audio_track_method_id, 44100,
                        player->out_channel);
    player->play_audio_track_method_id = env->GetMethodID(player_class, "playAudioTrack", "([BI)V");
    return 1;
}

void video_play(Player *player, AVFrame *frame, JNIEnv *env) {
    int video_height = player->video_codec_context->height;
    int result = sws_scale(player->sws_context, (const uint8_t *const *) frame->data,
                           frame->linesize, 0, video_height, player->rgba_frame->data,
                           player->rgba_frame->linesize);
    if (result <= 0) {
        LOGI("Player Error : video data convert fail");
        return;
    }
    result = ANativeWindow_lock(player->native_window, &(player->window_buffer), nullptr);
    if (result < 0) {
        LOGI("Player Error : Can not lock native window");
    } else {
        uint8_t *bits = (uint8_t *) (player->window_buffer.bits);
        for (int h = 0; h < video_height; ++h) {
            memcpy(bits + h * player->window_buffer.stride * 4,
                   player->video_out_buffer + h * player->rgba_frame->linesize[0],
                   player->rgba_frame->linesize[0]);

        }
        ANativeWindow_unlockAndPost(player->native_window);
    }
}

void audio_play(Player *player, AVFrame *frame, JNIEnv *env) {
    swr_convert(player->swr_context, &(player->audio_out_buffer), 44100 * 2,
                (const uint8_t **) frame->data, frame->nb_samples);
    int size = av_samples_get_buffer_size(nullptr, player->out_channel, frame->nb_samples,
                                          AV_SAMPLE_FMT_S16, 1);
    jbyteArray audio_sample_array = env->NewByteArray(size);
    env->SetByteArrayRegion(audio_sample_array, 0, size,
                            reinterpret_cast<const jbyte *>(player->audio_out_buffer));
    env->CallVoidMethod(player->instance, player->play_audio_track_method_id, audio_sample_array,
                        size);
    env->DeleteLocalRef(audio_sample_array);
}

/**
 * 释放播放器
 * @param player
 */
void player_release(Player *player) {
    avformat_close_input(&(player->format_context));
    av_free(player->video_out_buffer);
    av_free(player->audio_out_buffer);
    avcodec_close(player->video_codec_context);
    ANativeWindow_release(player->native_window);
    sws_freeContext(player->sws_context);
    av_frame_free(&(player->rgba_frame));
    avcodec_close(player->audio_codec_context);
    swr_free(&(player->swr_context));
    queue_destroy(player->video_queue);
    queue_destroy(player->audio_queue);
    player->instance = NULL;
    JNIEnv *env;
    int result = player->java_vm->AttachCurrentThread(&env, NULL);
    if (result != JNI_OK) {
        return;
    }
    env->DeleteGlobalRef(player->instance);
    env->DeleteGlobalRef(player->surface);
    player->java_vm->DetachCurrentThread();
}

void play_start(Player *player) {
    player->audio_clock = 0;
    player->video_queue = (Queue *) malloc(sizeof(Queue));
    player->audio_queue = (Queue *) malloc(sizeof(Queue));
    queue_init(player->video_queue, "video");
    queue_init(player->audio_queue, "audio");
    thread_init(player);
}

void *produce(Player *player, int* i) {
    LOGI("produce i = %d", *i);
    AVPacket *packet = av_packet_alloc();
    for (;;) {
        int result = av_read_frame(player->format_context, packet);
        if (result < 0) {
            LOGI("avreadframe is less 0");
            break;
        }
        if (packet->stream_index == player->video_stream_index) {
            queue_in(player->video_queue, packet);
        } else if (packet->stream_index == player->audio_stream_index) {
            queue_in(player->audio_queue, packet);
        }
        packet = av_packet_alloc();
    }
    break_block(player->video_queue);
    break_block(player->audio_queue);
    for (;;) {
        if (queue_is_empty(player->video_queue) && queue_is_empty(player->audio_queue)) {
            break;
        }
    }
    player_release(player);
    return nullptr;
}

void *consumer(Consumer *consumer) {
    Player *player = consumer->player;
    int index = consumer->stream_index;
    JNIEnv *env = GetJniEnv();
    if (env == nullptr) {
        LOGI("jni is null , return");
        return nullptr;
    }
    AVCodecContext *codec_context;
    AVStream *stream;
    Queue *queue;
    int result = -1;
    if (index == player->video_stream_index) {
        codec_context = player->video_codec_context;
        stream = player->format_context->streams[player->video_stream_index];
        queue = player->video_queue;
        video_prepare(player, env);
    } else if (index == player->audio_stream_index) {
        codec_context = player->audio_codec_context;
        stream = player->format_context->streams[player->audio_stream_index];
        queue = player->audio_queue;
        audio_prepare(player, env);
    }
    double tatal = stream->duration * av_q2d(stream->time_base);
    AVFrame *frame = av_frame_alloc();
    for (;;) {
        AVPacket *packet = queue_out(queue);
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
            if (index == player->video_stream_index) {
                double audio_clock = player->audio_clock;
                double timestamp;
                if (packet->pts == AV_NOPTS_VALUE) {
                    timestamp = 0;
                } else {
                    timestamp = av_frame_get_best_effort_timestamp(frame) * av_q2d(stream->time_base);
                }
                double frame_rate = av_q2d(stream->avg_frame_rate);
                frame_rate += frame->repeat_pict * (frame_rate * 0.5);
                if (timestamp == 0.0) {
                    usleep((unsigned long) (frame_rate * 1000));
                } else {
                    if (fabs(timestamp - audio_clock) > AV_SYNC_THRESHOLD_MIN &&
                        fabs(timestamp - audio_clock) < AV_SYNC_THRESHOLD_MAX) {
                        if (timestamp > audio_clock) {
                            usleep((unsigned long) ((timestamp - audio_clock) * 1000000));
                        }
                    }
                }
                video_play(player, frame, env);
            } else if (index == player->audio_stream_index) {
                player->audio_clock = packet->pts * av_q2d(stream->time_base);
                audio_play(player, frame, env);
            }
        }
        av_packet_unref(packet);
    }
    player->java_vm->DetachCurrentThread();
    return nullptr;
}

std::thread produceT, video_consumerT, audio_consumerT;

void thread_init(Player *player) {
    int i = 10;
    produceT = std::thread(produce, player, &i);

    Consumer *video_consumer = (Consumer*) malloc(sizeof(Consumer));
    video_consumer->player = player;
    video_consumer->stream_index = player->video_stream_index;
    video_consumerT = std::thread(consumer, video_consumer);
    LOGI("video");

    Consumer *audio_consumer = (Consumer*) malloc(sizeof(Consumer));
    audio_consumer->player = player;
    audio_consumer->stream_index = player->audio_stream_index;
    audio_consumerT = std::thread(consumer, audio_consumer);
    LOGI("audio");
}