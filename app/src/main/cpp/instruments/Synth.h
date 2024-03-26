#ifndef INFLUX2_SYNTH_H
#define INFLUX2_SYNTH_H


#include "../AudioNode.h"

class Synth : public AudioNode {

public:
    bool playing = false;
    float amplitude = 0.0;
    float frequency = 440.0;
    uint64_t sampleCount = 0;


    void handleOutputAudio(int32_t *buffer, int32_t bufferSize) override;

    void handleMidi(int32_t *midiData, int32_t midiDataLength) override;

    bool handlesMidi() override;

    bool handlesOutputAudio() override;
};


#endif //INFLUX2_SYNTH_H
