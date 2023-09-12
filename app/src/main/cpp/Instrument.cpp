#include "Instrument.h"

Instrument::Instrument() {
    _audioNodes[0] = new Synth();
    _audioNodes[1] = new Spring();
}

void Instrument::onAudioInput(int32_t *buffer, int32_t bufferSize) {
    for(int i = 0; i < AUDIO_NODE_AMOUNT; i++) {
        if(_audioNodes[i]->handlesInputAudio()) {
            _audioNodes[i]->handleInputAudio(buffer, bufferSize);
        }
    }
}

void Instrument::onAudioOutput(int32_t *buffer, int32_t bufferSize) {
    for(int i = 0; i < AUDIO_NODE_AMOUNT; i++) {
        if(_audioNodes[i]->handlesOutputAudio()) {
            _audioNodes[i]->handleOutputAudio(buffer, bufferSize);
        }
    }
}

void Instrument::onMidi(int32_t *midiData, int32_t midiDataLength) {
    for(int i = 0; i < AUDIO_NODE_AMOUNT; i++) {
        if(_audioNodes[i]->handlesOutputAudio()) {
            _audioNodes[i]->handleMidi(midiData, midiDataLength);
        }
    }
    //TODO: Receive midi events to change _instrument, _effects, (envelope type), and modulator
}

AudioNode *Instrument::getAudioNode(int nodeIndex) {
    return _audioNodes[nodeIndex];
}

void Instrument::replaceAudioNode(int oldNodeIndex, int newNodeId) {
    delete _audioNodes[oldNodeIndex];
    _audioNodes[oldNodeIndex] = newAudioNode(newNodeId);
}

AudioNode* Instrument::newAudioNode(int audioNodeId) {
    switch(audioNodeId) {
        case 0:
            return new Synth();
        case 1:
            return new Spring();
        default:
            return new Synth();
    }
}
