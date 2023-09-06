#include "InstrumentPipeline.h"

InstrumentPipeline::InstrumentPipeline() {

}

void InstrumentPipeline::onAudioInput(int32_t *buffer, int32_t bufferSize) {
    if(instrument.takesAudio()) {
        instrument.onAudioInput(buffer, bufferSize);
    }
}

void InstrumentPipeline::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    if(instrument.sendsAudio()) {
        instrument.onAudioOutput(buffer, bufferSize);
    }
}

void InstrumentPipeline::onMidi(int32_t *midiData, int32_t midiDataLength) {
    if(instrument.takesMidi()) {
       instrument.onMidi(midiData, midiDataLength);
    }
}
