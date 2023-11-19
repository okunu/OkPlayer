//
// Created by okunu on 2023/11/18.
//

#ifndef OKPLAYER_AUDIOPLAYER_H
#define OKPLAYER_AUDIOPLAYER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <functional>

class AudioPlayer {

public:
    using AudioCallback = std::function<int()>;
    AudioPlayer();
    ~AudioPlayer();
    void init();
    void setAudioData(uint8_t *data, int size);
    int getSize() { return size_; }
    uint8_t *getBuffer() { return out_buffers; }
    void setCallback(AudioCallback callback) { callback_ = callback; }
    int invokeCallback() {
        return callback_();
    }
public:
    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;
private:
    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    uint8_t *out_buffers = 0;
    int size_ = 0;

    AudioCallback callback_;
};

#endif //OKPLAYER_AUDIOPLAYER_H
