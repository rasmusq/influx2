#ifndef INFLUX2_INSTRUMENT_H
#define INFLUX2_INSTRUMENT_H


#include "AudioNode.h"

class Instrument : AudioNode {
    virtual void onAudioOutput(int32_t *buffer, int32_t bufferSize) = 0;
    virtual void onAudioInput(int32_t *buffer, int32_t bufferSize) = 0;
    virtual void onMidi(int32_t *midiData, int32_t midiDataLength) = 0;

    virtual bool sendsAudio() { return true; }
    virtual bool takesAudio() = 0;
    virtual bool takesMidi() = 0;
};


#endif //INFLUX2_INSTRUMENT_H
