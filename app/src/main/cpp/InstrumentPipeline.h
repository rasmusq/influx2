#ifndef INFLUX2_INSTRUMENTPIPELINE_H
#define INFLUX2_INSTRUMENTPIPELINE_H


#include <cstdint>
#include "AudioNode.h"
#include "instruments/Synth.h"
#include "effects/Spring.h"

class InstrumentPipeline {

    AudioNode* instrument = new Synth();
    int currentInstrumentIndex;
    int currentEffectIndex;
    const static int EFFECT_AMOUNT = 4;
//    AudioNode effects[EFFECT_AMOUNT];
    AudioNode* effect = new Spring();

public:
    InstrumentPipeline();

    void onAudioInput(int32_t *buffer, int32_t bufferSize);

    void onAudioOutput(int32_t *buffer, int32_t bufferSize);

    void onMidi(int32_t *midiData, int32_t midiDataLength);


};


#endif //INFLUX2_INSTRUMENTPIPELINE_H
