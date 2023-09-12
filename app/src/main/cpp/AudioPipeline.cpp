#include <android/log.h>
#include "AudioPipeline.h"

AudioPipeline::AudioPipeline() {
    _currentInstrumentIndex = 0;
    for (int i = 0; i < MAX_PIPELINES; i++) {
        _instruments[i] = new Instrument();
    }
    _currentInstrumentPtr = _instruments[_currentInstrumentIndex];
}

void AudioPipeline::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    _currentInstrumentPtr->onAudioOutput(buffer, bufferSize);
}

void AudioPipeline::onAudioInput(int32_t *buffer, int32_t bufferSize) {
    _currentInstrumentPtr->onAudioInput(buffer, bufferSize);
}

void AudioPipeline::onMidi(int32_t *midiData, int32_t midiDataLength) {
    _currentInstrumentPtr->onMidi(midiData, midiDataLength);
    __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                        "Midi: %d", midiData[midiDataLength - 1]);
}

Instrument *AudioPipeline::getInstrument(int index) {
    return _instruments[index];
}
Instrument *AudioPipeline::getCurrentInstrument() {
    return _currentInstrumentPtr;
}
