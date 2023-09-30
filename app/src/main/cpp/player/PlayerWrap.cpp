//
// Created by okunu on 2023/9/23.
//

#include "PlayerWrap.h"
#include "JniHelper.h"
#include "LogUtil.h"
#include <chrono>

PlayerWrap::PlayerWrap() : is_pause_(false) {
}

PlayerWrap::~PlayerWrap() {
    LOGI("PlayerWrap delete");
    //回收数据
    release();
}

int PlayerWrap::player_init(jobject instance_, jobject surface_, const char *path) {
    LOGI("player_init");
    auto env = GetJniEnv();
    instance = env->NewGlobalRef(instance_);
    surface = env->NewGlobalRef(surface_);
    video_queue.start();
    audio_queue.start();

    is_pause_ = false;

    int result = 1;
    result = format_init(path);
    if (result > 0) {
        result = codec_init(AVMEDIA_TYPE_VIDEO);
    }
    if (result > 0) {
        result = codec_init(AVMEDIA_TYPE_AUDIO);
    }
    return result;
}

int PlayerWrap::format_init(const char *path) {
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

int PlayerWrap::find_stream_index(AVMediaType type) {
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


int PlayerWrap::codec_init(AVMediaType type) {
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
    if (type == AVMEDIA_TYPE_AUDIO) {
        audio_stream_index = index;
        audio_codec_context = codec_context;
    } else if (type == AVMEDIA_TYPE_VIDEO) {
        video_stream_index = index;
        video_codec_context = codec_context;
    }
    return 1;
}

int PlayerWrap::video_prepare() {
    LOGI("video_prepare");
    AVCodecContext *codec_context = video_codec_context;
    int videoWidth = codec_context->width;
    int videoHeight = codec_context->height;
    auto env = GetJniEnv();
    native_window = ANativeWindow_fromSurface(env, surface);
    if (native_window == NULL) {
        LOGI("Player Error : Can not create native window");
        return FAIL_CODE;
    }
    int result = ANativeWindow_setBuffersGeometry(native_window, videoWidth, videoHeight,
                                                  WINDOW_FORMAT_RGBA_8888);
    if (result < 0) {
        LOGI("Player Error : Can not set native window buffer");
        ANativeWindow_release(native_window);
        return FAIL_CODE;
    }
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

int PlayerWrap::audio_prepare() {
    LOGI("audio_prepare");
    auto env = GetJniEnv();
    AVCodecContext *codec_context = audio_codec_context;
    swr_context = swr_alloc();
    audio_out_buffer = (uint8_t *) av_malloc(44100 * 2);
    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_format = AV_SAMPLE_FMT_S16;
    int out_sample_rate = audio_codec_context->sample_rate;
    swr_alloc_set_opts(swr_context,
                       out_channel_layout, out_format, out_sample_rate,
                       codec_context->channel_layout, codec_context->sample_fmt,
                       codec_context->sample_rate,
                       0, NULL);
    swr_init(swr_context);
    out_channel = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    jclass player_class = env->GetObjectClass(instance);
    jmethodID create_audio_track_method_id = env->GetMethodID(player_class, "createAudioTrack",
                                                              "(II)V");
    env->CallVoidMethod(instance, create_audio_track_method_id, 44100,
                        out_channel);
    play_audio_track_method_id = env->GetMethodID(player_class, "playAudioTrack", "([BI)V");
    return 1;
}

void PlayerWrap::video_play(AVFrame *frame) {
//    LOGI("video_play");
    int video_height = video_codec_context->height;
    int result = sws_scale(sws_context, (const uint8_t *const *) frame->data,
                           frame->linesize, 0, video_height, rgba_frame->data,
                           rgba_frame->linesize);
    if (result <= 0) {
        LOGI("Player Error : video data convert fail");
        return;
    }
    result = ANativeWindow_lock(native_window, &(window_buffer), nullptr);
    if (result < 0) {
        LOGI("Player Error : Can not lock native window");
    } else {
        uint8_t *bits = (uint8_t *) (window_buffer.bits);
        for (int h = 0; h < video_height; ++h) {
            memcpy(bits + h * window_buffer.stride * 4,
                   video_out_buffer + h * rgba_frame->linesize[0],
                   rgba_frame->linesize[0]);

        }
        ANativeWindow_unlockAndPost(native_window);
    }
}

void PlayerWrap::audio_play(AVFrame *frame) {
//    LOGI("audio_play");
    auto env = GetJniEnv();
    swr_convert(swr_context, &(audio_out_buffer), 44100 * 2,
                (const uint8_t **) frame->data, frame->nb_samples);
    int size = av_samples_get_buffer_size(nullptr, out_channel, frame->nb_samples,
                                          AV_SAMPLE_FMT_S16, 1);
    jbyteArray audio_sample_array = env->NewByteArray(size);
    env->SetByteArrayRegion(audio_sample_array, 0, size,
                            reinterpret_cast<const jbyte *>(audio_out_buffer));
    env->CallVoidMethod(instance, play_audio_track_method_id, audio_sample_array,
                        size);
    env->DeleteLocalRef(audio_sample_array);
}

void PlayerWrap::release() {
    LOGI("player_release");
    avformat_close_input(&(format_context_));
    avformat_free_context(format_context_);
    format_context_ = nullptr;
    av_free(video_out_buffer);
    av_free(audio_out_buffer);
    avcodec_close(video_codec_context);
    avcodec_free_context(&video_codec_context);
    video_codec_context = nullptr;

    ANativeWindow_release(native_window);
    native_window = nullptr;

    sws_freeContext(sws_context);
    av_frame_free(&(rgba_frame));

    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);
    audio_codec_context = nullptr;

    swr_free(&(swr_context));
    JNIEnv *env = GetJniEnv();
    env->DeleteGlobalRef(instance);
    env->DeleteGlobalRef(surface);
    JavaVM *vm;
    env->GetJavaVM(&vm);
    if (vm != nullptr) {
        vm->DetachCurrentThread();
    }
}

void PlayerWrap::play_start() {
    LOGI("play_start");
    audio_clock_ = 0;
    pool_.submit("produce", [&]() {
        produce();
    });
    pool_.submit("video consumer", [&](int index) {
        consumer(index);
    }, video_stream_index);
    pool_.submit("audio consumer", [&](int index) {
        consumer(index);
    }, audio_stream_index);

//    produceT = std::thread([&](){
//        produce();
//    });
//
//    video_consumerT = std::thread([&](int index){
//        consumer(index);
//    }, video_stream_index);
//
//    audio_consumerT = std::thread([&](int index){
//        consumer(index);
//    }, audio_stream_index);
}

void PlayerWrap::pause() {
    lock_guard<mutex> lock(mutex_);
    is_pause_ = !is_pause_;
    LOGI("is pause: %d", is_pause_);
    cv_.notify_one();
}

void PlayerWrap::produce() {
    LOGI("produce");
    AVPacket *packet = av_packet_alloc();
    for (;;) {
        {
            unique_lock<mutex> lock(mutex_);
            while (is_pause_) {
                cv_.wait(lock, [this]() { return !is_pause_; });
            }
        }
        int result = av_read_frame(format_context_, packet);
        if (result < 0) {
            LOGI("avreadframe is less 0");
            break;
        }
        if (packet->stream_index == video_stream_index) {
            video_queue.push(packet);
        } else if (packet->stream_index == audio_stream_index) {
            audio_queue.push(packet);
        }
        packet = av_packet_alloc();
    }
    LOGI("produce video size = %d", video_queue.size());
    LOGI("produce audio size = %d", audio_queue.size());
    video_queue.stop();
    audio_queue.stop();
    for (;;) {
        if (video_queue.is_empty() && audio_queue.is_empty()) {
            LOGI("video queue is empty and audio queue is empty");
            break;
        }
    }
    release();
    LOGI("produce end");
}

void PlayerWrap::consumer(int index) {
    LOGI("consumer index = %d videoIndex = %d", index, video_stream_index);
    JNIEnv *env = GetJniEnv();
    if (env == nullptr) {
        LOGI("jni is null , return");
        return;
    }
    AVCodecContext *codec_context;
    AVStream *stream;
    BlockQueue<Element> *queue;
    int result = -1;
    if (index == video_stream_index) {
        codec_context = video_codec_context;
        stream = format_context_->streams[video_stream_index];
        queue = &video_queue;
        video_prepare();
    } else if (index == audio_stream_index) {
        codec_context = audio_codec_context;
        stream = format_context_->streams[audio_stream_index];
        queue = &audio_queue;
        audio_prepare();
    }
    double tatal = stream->duration * av_q2d(stream->time_base);
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
            if (index == video_stream_index) {
                double audio_clock = audio_clock_;
                double timestamp;
                if (packet->pts == AV_NOPTS_VALUE) {
                    timestamp = 0;
                } else {
                    timestamp =
                            av_frame_get_best_effort_timestamp(frame) * av_q2d(stream->time_base);
                }
                double frame_rate = av_q2d(stream->avg_frame_rate);
                frame_rate += frame->repeat_pict * (frame_rate * 0.5);
                //LOGI("timestamp = %f", timestamp);
                if (timestamp == 0.0) {
                    usleep((unsigned long) (frame_rate * 1000));
//                    std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)frame_rate));
                } else {
                    if (fabs(timestamp - audio_clock) > AV_SYNC_THRESHOLD_MIN &&
                        fabs(timestamp - audio_clock) < AV_SYNC_THRESHOLD_MAX) {
                        if (timestamp > audio_clock) {
                            usleep((unsigned long) ((timestamp - audio_clock) * 1000000));
//                            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long) ((timestamp - audio_clock) * 1000)));
                        }
                    }
                }
                video_play(frame);
            } else if (index == audio_stream_index) {
                audio_clock_ = packet->pts * av_q2d(stream->time_base);
                audio_play(frame);
            }
        }
        av_packet_unref(packet);
    }
    LOGI("consumer end index = %d  audio_queue.size = %d  video_queue.size = %d", index,
         audio_queue.size(), video_queue.size());
    JavaVM *vm;
    env->GetJavaVM(&vm);
    if (vm != nullptr) {
        vm->DetachCurrentThread();
    }
    return;
}