#ifndef INFLUX2_AUDIOPIPELINE_H
#define INFLUX2_AUDIOPIPELINE_H

#include <stdint.h>
#include <vector>
#include "InstrumentPipeline.h"

class AudioPipeline {

    const static int MAX_PIPELINES = 4;
    InstrumentPipeline instrumentPipelines[MAX_PIPELINES];
    int currentPipelineIndex;
    InstrumentPipeline *currentPipelinePtr;

public:
    AudioPipeline();
    void onAudioInput(int32_t *buffer, int32_t bufferSize);
    void onAudioOutput(int32_t *buffer, int32_t bufferSize);
    void onMidi(int32_t *midiData, int32_t midiDataLength);
};


#endif //INFLUX2_AUDIOPIPELINE_H
