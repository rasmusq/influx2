#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include <android/log.h>
#include <unistd.h>
#include <math.h>
#include <chrono>
#include "AudioPipeline.h"

int inputBufferSize = 0;
int outputBufferSize = 0;
int inputSampleRate = 0;
int outputSampleRate = 0;
//NOTE: We assume that there are always two channels, because that is what we set it to

AudioPipeline audioPipeline = AudioPipeline();

class AudioOutputCallback : public oboe::AudioStreamDataCallback {
public:
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
        auto *outputData = static_cast<int32_t *>(audioData);
        audioPipeline.onAudioOutput(outputData, numFrames);
        return oboe::DataCallbackResult::Continue;
    }
};

class AudioInputCallback : public oboe::AudioStreamDataCallback {
public:
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
        auto *inputData = static_cast<int32_t *>(audioData);
        audioPipeline.onAudioInput(inputData, numFrames);
        return oboe::DataCallbackResult::Continue;
    }
};

AudioOutputCallback audioOutputCallback;
AudioInputCallback audioInputCallback;
oboe::AudioStreamBuilder builder;
std::shared_ptr<oboe::AudioStream> audioOutputStream;
std::shared_ptr<oboe::AudioStream> audioInputStream;

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_initAudioStream(
        JNIEnv *env,
        jobject /* this */) {

    // Output
    builder.setDirection(oboe::Direction::Output)->
            setPerformanceMode(oboe::PerformanceMode::LowLatency)->
            setSharingMode(oboe::SharingMode::Exclusive)->
            setFormat(oboe::AudioFormat::I32)->
            setChannelCount(oboe::ChannelCount::Stereo)->
            setDataCallback(&audioOutputCallback);


    oboe::Result result = builder.openStream(audioOutputStream);

    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create output stream. Error: %s",
                            oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created output stream. Result: %s",
                            oboe::convertToText(result));
    }

    oboe::AudioFormat format = audioOutputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Output AudioStream format is %s",
                        oboe::convertToText(format));
    outputSampleRate = audioOutputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Output AudioStream sampleRate is %d", outputSampleRate);
    outputBufferSize = audioOutputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Output AudioStream bufferSize is %d", outputBufferSize);
    int32_t channelCount = audioOutputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Output AudioStream channelCount is %d", channelCount);



    // Input
    builder.setDirection(oboe::Direction::Input)->setDataCallback(&audioInputCallback);
    result = builder.openStream(audioInputStream);
    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create input stream. Error: %s",
                            oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created input stream. Result: %s",
                            oboe::convertToText(result));
    }

    format = audioInputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Input AudioStream format is %s",
                        oboe::convertToText(format));
    inputSampleRate = audioInputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Input AudioStream sampleRate is %d", inputSampleRate);
    inputBufferSize = audioInputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Input AudioStream bufferSize is %d", inputBufferSize);
    channelCount = audioInputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Input AudioStream channelCount is %d", channelCount);

    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_startAudioStream(JNIEnv *env, jobject /* this */) {
    audioOutputStream->requestStart();
    audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Started");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_stopAudioStream(JNIEnv *env, jobject /* this */) {
    audioOutputStream->requestStop();
    audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Stopped");
    return 0;
}
extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_closeAudioStream(JNIEnv *env, jobject /* this */) {
    audioOutputStream->close();
    audioInputStream->requestStart();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Closed");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_onMidi(JNIEnv *env, jobject /* this */, jintArray midiData) {
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Midi Sent");
    jint *jintArray = env->GetIntArrayElements(midiData, nullptr);
    int arrayLength = env->GetArrayLength(midiData);

    // Create a C array of int
    int *cArray = new int[arrayLength];

    // Convert each element from jint to int
    for (int i = 0; i < arrayLength; i++) {
        cArray[i] = static_cast<int>(jintArray[i]);
    }

    // Now, 'cArray' is your C array of integers (int[]).

    // Perform operations on 'cArray' as needed.
    audioPipeline.onMidi(cArray, arrayLength);

    // Don't forget to release the 'jintArray' and 'cArray' when you're done.
    env->ReleaseIntArrayElements(midiData, jintArray, JNI_ABORT);
    delete[] cArray;
    return 0;
}