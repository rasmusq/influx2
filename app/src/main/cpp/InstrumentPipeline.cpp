#include "InstrumentPipeline.h"

InstrumentPipeline::InstrumentPipeline() {

}

void InstrumentPipeline::onAudioInput(int32_t *buffer, int32_t bufferSize) {
    if(instrument->handlesInputAudio()) {
        instrument->handleInputAudio(buffer, bufferSize);
    }
    if(effect->handlesInputAudio()) {
        effect->handleInputAudio(buffer, bufferSize);
    }
}

void InstrumentPipeline::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    if(instrument->handlesOutputAudio()) {
        instrument->handleOutputAudio(buffer, bufferSize);
    }
    if(effect->handlesOutputAudio()) {
        effect->handleOutputAudio(buffer, bufferSize);
    }
}

void InstrumentPipeline::onMidi(int32_t *midiData, int32_t midiDataLength) {
    if(instrument->handlesMidi()) {
        instrument->handleMidi(midiData, midiDataLength);
    }
    if(effect->handlesMidi()) {
        effect->handleMidi(midiData, midiDataLength);
    }
    //TODO: Receive midi events to change instrument, effects, (envelope type), and modulator
}
