//
// Created by okunu on 2023/11/18.
//

#include "AudioPlayer.h"
#include "LogUtil.h"

AudioPlayer::AudioPlayer() {}

AudioPlayer::~AudioPlayer() {}

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    AudioPlayer *player = static_cast<AudioPlayer *>(context);
    int size = player->invokeCallback();
//    LOGI("bqPlayerCallback size = %d",size);
    if (size > 0) {
        (*player->pcmBufferQueue)->Enqueue(player->pcmBufferQueue, player->getBuffer(), size);
    }
}

void AudioPlayer::setAudioData(uint8_t *data, int size) {
    out_buffers = data;
    size_ = size;
}

void AudioPlayer::init() {
    SLresult result;
    result = slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

    //第二步，创建混音器
    const SLInterfaceID mids[2] = {SL_IID_ENVIRONMENTALREVERB, SL_IID_PLAYBACKRATE};
    const SLboolean mreq[2] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 2, mids,
                                              mreq);
    (void) result;
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (void) result;
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        (void) result;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&outputMix, 0};


    // 第三步，配置PCM格式信息
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//播放pcm格式的数据
            2,//2个声道（立体声）
            SL_SAMPLINGRATE_44_1,//44100hz的频率
            SL_PCMSAMPLEFORMAT_FIXED_16,//位数 16位
            SL_PCMSAMPLEFORMAT_FIXED_16,//和位数一致就行
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,//立体声（前左前右）
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };
    SLDataSource slDataSource = {&android_queue, &pcm};


    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource,
                                       &audioSnk, 1, ids, req);
    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);

    //得到接口后调用  获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

    //注册回调缓冲区 获取缓冲队列接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE,
                                     &pcmBufferQueue);
    //缓冲接口回调
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, bqPlayerCallback, this);
    //获取播放状态接口
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    bqPlayerCallback(pcmBufferQueue, this);
}
