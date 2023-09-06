#ifndef INFLUX2_AUDIONODE_H
#define INFLUX2_AUDIONODE_H

#include <cstdint>

class AudioNode {

int audioDataArrayIndex = 0;

public:
    static const int AUDIO_BUFFER_LENGTH = 1024;
    static const int MAX_VALUE_COUNT = 16;

    int valueData[MAX_VALUE_COUNT]; // Data for the controlled values of the
    int audioData[AUDIO_BUFFER_LENGTH]; // Data for visualizing the waveform of the node (Can be used as double buffer)
    int peakValue; // For getting the peak of the output to visualize output level
    int *otherData; // Lets the node compile its own proprietary data for the frontend to read

    virtual void onAudioOutput(int32_t *buffer, int32_t bufferSize) = 0;
    virtual void onAudioInput(int32_t *buffer, int32_t bufferSize) = 0;
    virtual void onMidi(int32_t *midiData, int32_t midiDataLength) = 0;

    virtual bool takesMidi() = 0;
    virtual bool takesAudio() = 0;
    virtual bool sendsAudio() = 0;
    virtual bool sendsMidi() = 0;

    void readSample(int32_t sample); // Used for reading a sample for storing data about waveform and peaks
};


#endif //INFLUX2_AUDIONODE_H
