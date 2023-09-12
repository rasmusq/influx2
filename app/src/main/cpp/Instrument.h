#ifndef INFLUX2_INSTRUMENT_H
#define INFLUX2_INSTRUMENT_H


#include <cstdint>
#include "AudioNode.h"
#include "instruments/Synth.h"
#include "effects/Spring.h"

class Instrument {

public:
    const static int AUDIO_NODE_AMOUNT = 2;

    Instrument();

    void onAudioInput(int32_t *buffer, int32_t bufferSize);

    void onAudioOutput(int32_t *buffer, int32_t bufferSize);

    void onMidi(int32_t *midiData, int32_t midiDataLength);

    AudioNode *getAudioNode(int nodeIndex);

    void replaceAudioNode(int oldNodeIndex, int newNodeId);

    AudioNode *newAudioNode(int audioNodeId);

private:
    AudioNode *_audioNodes[AUDIO_NODE_AMOUNT];

};


#endif //INFLUX2_INSTRUMENT_H
