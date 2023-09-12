#ifndef INFLUX2_AUDIOPIPELINE_H
#define INFLUX2_AUDIOPIPELINE_H

#include <stdint.h>
#include <vector>
#include "Instrument.h"

class AudioPipeline {

public:
    const static int MAX_PIPELINES = 4;
    AudioPipeline();
    void onAudioInput(int32_t *buffer, int32_t bufferSize);
    void onAudioOutput(int32_t *buffer, int32_t bufferSize);
    void onMidi(int32_t *midiData, int32_t midiDataLength);


    Instrument *getInstrument(int index);
    Instrument *getCurrentInstrument();

private:
    Instrument *_instruments[MAX_PIPELINES];
    int _currentInstrumentIndex;
    Instrument *_currentInstrumentPtr;

};


#endif //INFLUX2_AUDIOPIPELINE_H
