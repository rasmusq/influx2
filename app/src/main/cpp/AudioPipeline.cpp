#include <android/log.h>
#include "AudioPipeline.h"

AudioPipeline::AudioPipeline() {
    currentPipelineIndex = 0;
    for (int i = 0; i < MAX_PIPELINES; i++) {
        instrumentPipelines[i] = InstrumentPipeline();
    }
    currentPipelinePtr = &instrumentPipelines[currentPipelineIndex];
}

void AudioPipeline::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    currentPipelinePtr->onAudioOutput(buffer, bufferSize);
}

void AudioPipeline::onAudioInput(int32_t *buffer, int32_t bufferSize) {
    currentPipelinePtr->onAudioInput(buffer, bufferSize);
}

void AudioPipeline::onMidi(int32_t *midiData, int32_t midiDataLength) {
    currentPipelinePtr->onMidi(midiData, midiDataLength);
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Midi: %d", midiData[midiDataLength - 1]);
}
