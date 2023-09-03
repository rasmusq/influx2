#include <jni.h>
#include <string>
#include <oboe/Oboe.h>
#include <android/log.h>
#include <unistd.h>
#include <math.h>

namespace AudioSystem {
    bool activated = true;
    int64_t sampleCount = 0;
}

class AudioCallback : public oboe::AudioStreamDataCallback {
public:

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {

        // We requested AudioFormat::Float. So if the stream opens
        // we know we got the Float format.
        // If you do not specify a format then you should check what format
        // the stream has and cast to the appropriate type.
        auto *outputData = static_cast<float *>(audioData);

        // Generate random numbers (white noise) centered around zero.
        const int32_t amplitude = 0.2 * INT16_MAX;
        for (int i = 0; i < numFrames; i += 2) {
            int32_t sample;
            //sample = (drand48() - 0.5f) * 2 * amplitude; // White Noise
            int32_t frequency = 440;//(sin((double)AudioSystem::sampleCount/48000.0) * 440.0 + 220.0);
            sample = sin((((AudioSystem::sampleCount)*frequency)/48000.0)/3.1415) * amplitude; // Sine Wave
            if(AudioSystem::sampleCount % 1000 == 0) {
                //__android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "SampleCount is %d", AudioSystem::sampleCount);
                __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Sample is %d", sample);
                //__android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "Frequency is %d", frequency);
            }
            //sample = AudioSystem::sampleCount % (long)(48000.0/frequency) < 200 ? 2000 : -2000;
            AudioSystem::sampleCount++;
            if (AudioSystem::activated) {
                outputData[i] = sample;
                outputData[i + 1] = sample;
            }
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
            setFormat(oboe::AudioFormat::I32)->
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
    int32_t sampleRate = AudioBridge::audioStream->getSampleRate();
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__, "AudioStream sampleRate is %d", sampleRate);

    AudioBridge::audioStream->requestStart();

    sleep(100);

    AudioBridge::audioStream->requestStop();
    AudioBridge::audioStream->close();


    __android_log_print(ANDROID_LOG_VERBOSE, "TEST", "SampleCount %d",
                        AudioSystem::sampleCount);

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