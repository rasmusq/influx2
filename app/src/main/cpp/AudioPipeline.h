#ifndef INFLUX2_AUDIOPIPELINE_H
#define INFLUX2_AUDIOPIPELINE_H

#include <stdint.h>
#include <list>
#include "InstrumentPipeline.h"

class AudioPipeline {

    std::list<InstrumentPipeline> instrumentPipelines;
    const int maxPipelines = 4;
    int currentPipeline;

public:
    AudioPipeline();
    void onAudioInput(int32_t *buffer, int32_t bufferSize);
    void onAudioOutput(int32_t *buffer, int32_t bufferSize);
    void onMidi(int32_t *midiData, int32_t midiDataLength);
};


#endif //INFLUX2_AUDIOPIPELINE_H
