#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include <android/log.h>
#include <unistd.h>
#include <math.h>
#include <chrono>

bool playing = false;
int64_t sampleCount = 0;
int64_t currentInputBufferSample = 0;
int32_t *inputBuffer;
int32_t *outputBuffer;
int inputBufferSize = 0;
int outputBufferSize = 0;

class AudioOutputCallback : public oboe::AudioStreamDataCallback {
public:

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {

        // We requested AudioFormat::Float. So if the stream opens
        // we know we got the Float format.
        // If you do not specify a format then you should check what format
        // the stream has and cast to the appropriate type.
        auto *outputData = static_cast<int32_t *>(audioData);
        outputBuffer = outputData;

        // Generate random numbers (white noise) centered around zero.
        const int32_t amplitude = INT32_MAX / 5;
        for (int i = 0; i < numFrames*2; i += 2) {
            sampleCount++;
            if (playing) {
//                outputData[i] = tan((double)(sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
//                outputData[i+1] = tan((double)(sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
                outputData[i] = inputBuffer[currentInputBufferSample];
                outputData[i+1] = inputBuffer[currentInputBufferSample+1];
                currentInputBufferSample += 2;
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
        inputBuffer = inputData;

        // Generate random numbers (white noise) centered around zero.
        currentInputBufferSample = 0;
        for (int i = 0; i < numFrames*2; i += 2) {

        }

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
            setDataCallback(&audioOutputCallback);


    oboe::Result result = builder.openStream(audioOutputStream);

    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create output stream. Error: %s", oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created output stream. Result: %s", oboe::convertToText(result));
    }

    oboe::AudioFormat format = audioOutputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream format is %s",
                        oboe::convertToText(format));
    int32_t sampleRate = audioOutputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream sampleRate is %d", sampleRate);
    outputBufferSize = audioOutputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream bufferSize is %d", outputBufferSize);
    int32_t channelCount = audioOutputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Output AudioStream channelCount is %d", channelCount);



    // Input
    builder.setDirection(oboe::Direction::Input)->setDataCallback(&audioInputCallback);
    result = builder.openStream(audioInputStream);
    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create input stream. Error: %s", oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created input stream. Result: %s", oboe::convertToText(result));
    }

    format = audioInputStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream format is %s",
                        oboe::convertToText(format));
    sampleRate = audioInputStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream sampleRate is %d", sampleRate);
    inputBufferSize = audioInputStream->getBufferSizeInFrames();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream bufferSize is %d", inputBufferSize);
    channelCount = audioInputStream->getChannelCount();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Input AudioStream channelCount is %d", channelCount);

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
Java_com_rasmusq_influx2_MainActivity_pauseAudio(JNIEnv *env, jobject /* this */) {
    playing = false;
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Paused");
    return 0;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_resumeAudio(JNIEnv *env, jobject /* this */) {
    playing = true;
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "AudioStream Resumed");
    return 0;
}

extern "C" JNIEXPORT jintArray JNICALL
Java_com_rasmusq_influx2_MainActivity_getInputBuffer(JNIEnv *env, jobject /* this */) {
    // Create a new jintArray
    jintArray resultArray = env->NewIntArray(inputBufferSize);
    // Check for exceptions
    if (resultArray == NULL || inputBuffer == NULL || inputBufferSize == 0) {
        // Handle error, return or throw an exception
        return NULL;
    }
    // Copy the contents of the int32_t array to the jintArray
    env->SetIntArrayRegion(resultArray, 0, inputBufferSize, reinterpret_cast<jint*>(inputBuffer));
    return resultArray;
}
extern "C" JNIEXPORT jintArray JNICALL
Java_com_rasmusq_influx2_MainActivity_getOutputBuffer(JNIEnv *env, jobject /* this */) {
    // Create a new jintArray
    jintArray resultArray = env->NewIntArray(outputBufferSize);
    // Check for exceptions
    if (resultArray == NULL || outputBuffer == NULL || outputBufferSize == 0) {
        // Handle error, return or throw an exception
        return NULL;
    }
    // Copy the contents of the int32_t array to the jintArray
    env->SetIntArrayRegion(resultArray, 0, outputBufferSize, reinterpret_cast<jint*>(outputBuffer));

    return resultArray;
}