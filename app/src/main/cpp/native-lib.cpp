#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include <android/log.h>
#include <unistd.h>
#include <math.h>
#include <chrono>

namespace AudioSystem {
    bool playing = false;
    int64_t sampleCount = 0;
    int64_t currentInputBufferSample = 0;
    int32_t *inputBuffer;
}

class AudioOutputCallback : public oboe::AudioStreamDataCallback {
public:

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {

        // We requested AudioFormat::Float. So if the stream opens
        // we know we got the Float format.
        // If you do not specify a format then you should check what format
        // the stream has and cast to the appropriate type.
        auto *outputData = static_cast<int32_t *>(audioData);

        // Generate random numbers (white noise) centered around zero.
        const int32_t amplitude = INT32_MAX / 5;
        for (int i = 0; i < numFrames*2; i += 2) {
            AudioSystem::sampleCount++;
            if (AudioSystem::playing) {
//                outputData[i] = tan((double)(AudioSystem::sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
//                outputData[i+1] = tan((double)(AudioSystem::sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
                outputData[i] = AudioSystem::inputBuffer[AudioSystem::currentInputBufferSample];
                outputData[i+1] = AudioSystem::inputBuffer[AudioSystem::currentInputBufferSample+1];
                AudioSystem::currentInputBufferSample += 2;
            } else {
                outputData[i] = 0;
                outputData[i+1] = 0;
            }
        }

        return oboe::DataCallbackResult::Continue;
    }
};

class AudioInputCallback : public oboe::AudioStreamDataCallback {
public:

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {

        // We requested AudioFormat::Float. So if the stream opens
        // we know we got the Float format.
        // If you do not specify a format then you should check what format
        // the stream has and cast to the appropriate type.
        auto *inputData = static_cast<int32_t *>(audioData);

        // Generate random numbers (white noise) centered around zero.
        AudioSystem::inputBuffer = inputData;
        AudioSystem::currentInputBufferSample = 0;
        for (int i = 0; i < numFrames*2; i += 2) {

        }

        return oboe::DataCallbackResult::Continue;
    }
};

namespace AudioBridge {
    AudioOutputCallback audioOutputCallback;
    AudioInputCallback audioInputCallback;
    oboe::AudioStreamBuilder builder;
    std::shared_ptr<oboe::AudioStream> audioOutputStream;
    std::shared_ptr<oboe::AudioStream> audioInputStream;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_initAudioStream(
        JNIEnv *env,
        jobject /* this */) {

    // Output
    AudioBridge::builder.setDirection(oboe::Direction::Output)->
            setPerformanceMode(oboe::PerformanceMode::LowLatency)->
            setSharingMode(oboe::SharingMode::Exclusive)->
            setFormat(oboe::AudioFormat::I32)->
            setDataCallback(&AudioBridge::audioOutputCallback);


    oboe::Result result = AudioBridge::builder.openStream(AudioBridge::audioOutputStream);

    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create output stream. Error: %s", oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created output stream. Result: %s", oboe::convertToText(result));
    }

    oboe::AudioFormat format = AudioBridge::audioOutputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream format is %s",
                        oboe::convertToText(format));
    int32_t sampleRate = AudioBridge::audioOutputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream sampleRate is %d", sampleRate);
    int32_t bufferSize = AudioBridge::audioOutputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream bufferSize is %d", bufferSize);
    int32_t channelCount = AudioBridge::audioOutputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream channelCount is %d", channelCount);



    // Input
    AudioBridge::builder.setDirection(oboe::Direction::Input)->setDataCallback(&AudioBridge::audioInputCallback);
    result = AudioBridge::builder.openStream(AudioBridge::audioInputStream);
    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create input stream. Error: %s", oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created input stream. Result: %s", oboe::convertToText(result));
    }

    format = AudioBridge::audioInputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream format is %s",
                        oboe::convertToText(format));
    sampleRate = AudioBridge::audioInputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream sampleRate is %d", sampleRate);
    bufferSize = AudioBridge::audioInputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream bufferSize is %d", bufferSize);
    channelCount = AudioBridge::audioInputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream channelCount is %d", channelCount);

    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_startAudioStream(JNIEnv *env, jobject /* this */) {
    AudioBridge::audioOutputStream->requestStart();
    AudioBridge::audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Started");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_stopAudioStream(JNIEnv *env, jobject /* this */) {
    AudioBridge::audioOutputStream->requestStop();
    AudioBridge::audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Stopped");
    return 0;
}
extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_closeAudioStream(JNIEnv *env, jobject /* this */) {
    AudioBridge::audioOutputStream->close();
    AudioBridge::audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Closed");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_pauseAudio(JNIEnv *env, jobject /* this */) {
    AudioSystem::playing = false;
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Closed");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_resumeAudio(JNIEnv *env, jobject /* this */) {
    AudioSystem::playing = true;
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Closed");
    return 0;
}