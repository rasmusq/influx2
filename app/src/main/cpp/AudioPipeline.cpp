#include <android/log.h>
#include "AudioPipeline.h"


int64_t sampleCount = 0;
bool playing = true;
int32_t amplitude = INT32_MAX / 5;

AudioPipeline::AudioPipeline() {
    currentPipeline = 0;
}

void AudioPipeline::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    for (int i = 0; i < bufferSize*2; i += 2) {
        if (playing) {
            buffer[i] = sin((double)(sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
            buffer[i+1] = sin((double)(sampleCount) * 440.0 * 3.141593 * 2.0 / 48000.0) * amplitude;
        } else {
            buffer[i] = 0;
            buffer[i+1] = 0;
        }
        sampleCount++;
    }
}

void AudioPipeline::onAudioInput(int32_t *buffer, int32_t bufferSize) {

}

void onMidi(int32_t *midiData, int32_t midiDataLength) {

}
