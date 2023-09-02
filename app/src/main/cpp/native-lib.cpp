#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include <android/log.h>
#include <unistd.h>
#include <math.h>

namespace AudioSystem {
    bool activated = false;
}

class AudioCallback : public oboe::AudioStreamDataCallback {
public:
    long leftSampleCount = 0;
    long rightSampleCount = 0;
    bool right = false;

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {

        // We requested AudioFormat::Float. So if the stream opens
        // we know we got the Float format.
        // If you do not specify a format then you should check what format
        // the stream has and cast to the appropriate type.
        auto *outputData = static_cast<float *>(audioData);

        // Generate random numbers (white noise) centered around zero.
        const float amplitude = 0.2f;
        for (int i = 0; i < numFrames; i += 2) {
            float sample;
            long *sampleCount = right ? &rightSampleCount : &leftSampleCount;
            sample = ((float) drand48() - 0.5f) * 2 * amplitude; // White Noise
            //sample = ((float)sin((float)*sampleCount/48000.0f*4400.0f) - 0.5f) * 2.0f * amplitude; // Sine Wave
            *sampleCount++;
            if (AudioSystem::activated) {
                outputData[i] = sample;
                outputData[i + 1] = sample;
            }
            __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                                "Audio Activated: %d", AudioSystem::activated);
        }

        return oboe::DataCallbackResult::Continue;
    }
};

namespace AudioBridge {
    AudioCallback audioCallback;
    oboe::AudioStreamBuilder builder;
    std::shared_ptr<oboe::AudioStream> audioStream;
}


extern "C" JNIEXPORT jint JNICALL
Java_com_rasmusq_influx2_MainActivity_initAudioStream(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    AudioBridge::builder.setDirection(oboe::Direction::Output)->
            setPerformanceMode(oboe::PerformanceMode::LowLatency)->
            setSharingMode(oboe::SharingMode::Exclusive)->
            setFormat(oboe::AudioFormat::Float)->
            setChannelCount(oboe::ChannelCount::Stereo)->
            setDataCallback(&AudioBridge::audioCallback);


    oboe::Result result = AudioBridge::builder.openStream(AudioBridge::audioStream);

    if (result != oboe::Result::OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Failed to create stream. Error: %s", oboe::convertToText(result));
        return 1;
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "Successfully created stream. Result: %s", oboe::convertToText(result));
    }

    oboe::AudioFormat format = AudioBridge::audioStream->getFormat();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "AudioStream format is %s",
                        oboe::convertToText(format));

    AudioBridge::audioStream->requestStart();

    sleep(10);

    AudioBridge::audioStream->requestStop();
    AudioBridge::audioStream->close();

    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_rasmusq_influx2_MainActivity_activateAudio(JNIEnv *env, jobject /* this */) {
    std::string hello = "hello";
    AudioSystem::activated = true;
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Audio Activated: %d", AudioSystem::activated);
    return env->NewStringUTF(hello.c_str());
}