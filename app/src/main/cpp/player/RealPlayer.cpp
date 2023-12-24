//
// Created by okunu on 2023/10/1.
//

#include "RealPlayer.h"
#include "LogUtil.h"

RealPlayer::RealPlayer() : display_(NULL) {
}

RealPlayer::~RealPlayer() {
    release();
}

int RealPlayer::player_init(const char *path) {
    video_queue.start();
    audio_queue.start();
    finish = false;
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
        video_width = video_codec_context->width;
        video_height = video_codec_context->height;
        adjustVideoScale();
    } else if (type == AVMEDIA_TYPE_AUDIO) {
        audio_stream_index = index;
        audio_codec_context = codec_context;
    }
    LOGI("audio index = %d, video index = %d", audio_stream_index, video_stream_index);
    return 1;
}

int RealPlayer::video_prepare() {
    LOGI("video_prepare");
    AVCodecContext *codec_context = video_codec_context;
    sws_context = sws_getContext(video_width, video_height, codec_context->pix_fmt,
                                 scale_video_width, scale_video_height, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
                                 nullptr, nullptr, nullptr);
    av_image_alloc(yuv_data, yuv_linesize, scale_video_width, scale_video_height, AV_PIX_FMT_YUV420P, 1);
    display_.bindCurrent();
    shader_.init();
    shader_.onSurfaceChanged(screen_width_, screen_height_, scale_video_width, scale_video_height);
    return 1;
}

void RealPlayer::video_play(AVFrame *frame) {
    double audio_clock = audio_clock_;
    double timestamp;
    auto stream = format_context_->streams[video_stream_index];
    timestamp = av_frame_get_best_effort_timestamp(frame) * av_q2d(stream->time_base);
    double frame_rate = av_q2d(stream->avg_frame_rate);
    frame_rate += frame->repeat_pict * (frame_rate * 0.5);
    //LOGI("timestamp = %f", timestamp);
    if (timestamp == 0.0) {
        usleep((unsigned long) (frame_rate * 1000));
//                    std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)frame_rate));
    } else {
        if (fabs(timestamp - audio_clock) > AV_SYNC_THRESHOLD_MIN &&
            fabs(timestamp - audio_clock) < AV_SYNC_THRESHOLD_MAX) {
            LOGI("time diff: %f", (timestamp - audio_clock));
            if (timestamp > audio_clock) {
                usleep((unsigned long) ((timestamp - audio_clock) * 1000000));
//                            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long) ((timestamp - audio_clock) * 1000)));
            }
        }
    }
    if (scale_video_width != video_width || scale_video_height != video_height) {
        sws_scale(sws_context, frame->data, frame->linesize, 0, video_height, yuv_data, yuv_linesize);
    }
    shader_.draw(yuv_data);
    display_.swapBuffer();
}

//此方法虽然暂时不用，但思路值得学习，如果要去除绿边，即编码自动添加的padding，需要通过转换一次宽高实现
void
RealPlayer::avFrameToYuvData(uint8_t **src_data, int src_width, int src_height, uint8_t **dst_data,
                             int *line_size) {
    uint32_t pitchY = src_width;
    uint32_t pitchU = src_width >> 1;
    uint32_t pitchV = src_width >> 1;

    uint8_t* avy = dst_data[0];
    uint8_t* avu = dst_data[1];
    uint8_t* avv = dst_data[2];

    for (int y_index = 0; y_index < src_height; ++y_index) {
        memcpy(avy, src_data[0] + y_index * line_size[0], pitchY);
        avy += pitchY;
    }

    for (int u_index = 0; u_index < src_height >> 1; ++u_index) {
        memcpy(avu, src_data[1] + u_index * line_size[1], pitchU);
        avu += pitchU;
    }

    for (int v_index = 0; v_index < src_height >> 1; ++v_index) {
        memcpy(avv, src_data[2] + v_index * line_size[2], pitchV);
        avv += pitchV;
    }
}

void RealPlayer::release() {
    LOGI("player_release");
    avformat_close_input(&(format_context_));
    avformat_free_context(format_context_);
    format_context_ = nullptr;
    avcodec_close(video_codec_context);
    avcodec_free_context(&video_codec_context);
    video_codec_context = nullptr;

    avcodec_close(audio_codec_context);
    avcodec_free_context(&audio_codec_context);
    audio_codec_context = nullptr;

    sws_freeContext(sws_context);

    av_free(yuv_data);
    av_free(audio_out_buffer);
}

void RealPlayer::play_start() {
    LOGI("play_start");
    audio_clock_ = 0;
    pool_.submit("produce", [&]() {
        produce();
    });
    pool_.submit("videoDecode", [&](int index) {
        videoDecode(index);
    }, video_stream_index);
    pool_.submit("audio consumer", [&]() {
        audioDecode();
    });
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
        } else if (packet->stream_index == audio_stream_index) {
            audio_queue.push(packet);
        }
        packet = av_packet_alloc();
    }
    LOGI("produce video size = %d  audio size = %d", video_queue.size(), audio_queue.size());
    LOGI("produce end");
}

int RealPlayer::audio_prepare() {
    AVCodecContext *codec_context = audio_codec_context;
    swr_context = swr_alloc();
    out_channel_layout_ = AV_CH_LAYOUT_STEREO;
    out_format_ = AV_SAMPLE_FMT_S16;
    out_sample_rate_ = audio_codec_context->sample_rate;
    out_nb_samples_ = av_rescale_rnd(audio_codec_context->frame_size,
                                     out_sample_rate_, audio_codec_context->sample_rate, AV_ROUND_UP);
    out_channels_ = av_get_channel_layout_nb_channels(out_channel_layout_);
    //这个buf可以弄大点，为了冗余，但这样算是最小数量
    audio_out_buffer = (uint8_t *) av_malloc(out_nb_samples_ * out_channels_);
    swr_alloc_set_opts(swr_context,
                       out_channel_layout_, out_format_, out_sample_rate_,
                       codec_context->channel_layout, codec_context->sample_fmt,
                       codec_context->sample_rate,
                       0, NULL);
    swr_init(swr_context);
    audioPlayer.setCallback([&]() {
        return this->audioDecodeOneFrame();
    });
    audioPlayer.init();
    return 1;
}

int RealPlayer::audioDecodeOneFrame() {
    int size = 0;
    while (true) {
        if (handleFinish()) {
            break;
        }
        size = 0;
        AVCodecContext *codec_context = audio_codec_context;
        BlockQueue<Element> *queue = &audio_queue;
        int result = -1;
        AVFrame *frame = av_frame_alloc();
        AVPacket *packet = queue->pop();
        if (packet == nullptr) {
            break;
        }
        //LOGI("packet: %d", packet->stream_index);
        result = avcodec_send_packet(codec_context, packet);
        if (result < 0 && result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
            LOGI("Player Error : codec step 1 fail audio");
            av_packet_free(&packet);
            return 0;
        }
        result = avcodec_receive_frame(codec_context, frame);
        //LOGI("ret = %d", ret);
        if (result == 0) {
            audio_clock_ = packet->pts * av_q2d(format_context_->streams[audio_stream_index]->time_base);
            int nb = swr_convert(swr_context, &(audio_out_buffer), out_nb_samples_,
                                 (const uint8_t **) frame->data, frame->nb_samples);
            size = nb * out_channels_ * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
            //LOGI("okunu channels = %d nb_samples: %d, channel_layout: %lu, count: %d", frame->channels, frame->nb_samples, frame->channel_layout, out_nb_samples_);
            audioPlayer.setAudioData(audio_out_buffer, size);
            av_packet_unref(packet);
            break;
        } else {
            LOGI("error and continue");
            continue;
        }
    }
    return size;
}

void RealPlayer::audioDecode() {
    audio_prepare();
    return;
}

void RealPlayer::adjustVideoScale() {
    if (video_width % 8 != 0) {
        scale_video_width = ((video_width / 8) + 1) * 8;
    } else {
        scale_video_width = video_width;
    }

    if (video_height % 2 != 0) {
        scale_video_height = ((video_height / 2) + 1) * 2;
    } else {
        scale_video_height = video_height;
    }
    LOGI("adjustVideoScale: vw: %d, vh: %d, sw: %d, sh: %d", video_width, video_height, scale_video_width, scale_video_height);
}

bool RealPlayer::handleFinish() {
    if (video_queue.is_empty() && audio_queue.is_empty()) {
        video_queue.stop();
        audio_queue.stop();
        finish = true;
        LOGI("finish: %d", finish);
        release();
    }
    return finish;
}

void RealPlayer::videoDecode(int index) {
    LOGI("videoDecode index = %d videoIndex = %d", index, video_stream_index);
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
        if (handleFinish()) {
            break;
        }
        AVPacket *packet = queue->pop();
        if (packet == nullptr) {
            LOGI("consume packet is null");
            break;
        }
        while (avcodec_receive_frame(codec_context, frame) == 0) {
            if (index == video_stream_index) {
                video_play(frame);
            }
        }
        result = avcodec_send_packet(codec_context, packet);
        if (result < 0 && result != AVERROR(EAGAIN) && result != AVERROR_EOF) {
            LOGI("Player Error : codec step 1 fail");
            av_packet_free(&packet);
            continue;
        }
        av_packet_unref(packet);
    }
    LOGI("videoDecode end index = %d video_queue.size = %d", index, video_queue.size());
    return;
}

void RealPlayer::surface_changed(int w, int h, EglDisplay &display) {
    screen_width_ = w;
    screen_height_ = h;
    display_ = display;
}