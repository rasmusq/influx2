#ifndef INFLUX2_AUDIONODE_H
#define INFLUX2_AUDIONODE_H

#include <cstdint>
#include <android/log.h>

class AudioNode {
public:
    //TODO: Make a system so both front- and backend know the selection of audioNodes
    static const int SYNTH_ID = 0;
    static const int SPRING_ID = 0;

    static const int AUDIO_DATA_ARRAY_LENGTH = 1024;
    static const int MAX_VALUE_COUNT = 16; // Each subclass determines amount below this count

    virtual void handleOutputAudio(int32_t *buffer, int32_t bufferSize) {
        // Do nothing (passthrough) if it is not implemented
    }

    virtual void handleInputAudio(int32_t *buffer, int32_t bufferSize) {
        // Do nothing if it is not implemented
    }

    virtual void handleMidi(int32_t *midiData, int32_t midiDataLength) {
        __android_log_print(ANDROID_LOG_ERROR, __FUNCTION__,
                            "MIDI not implemented for this AudioNode.");
    }

    virtual bool handlesMidi() { return false; };

    virtual bool handlesInputAudio() { return false; };

    virtual bool handlesOutputAudio() { return false; };

    void readSample(int32_t sampleL,
                    int32_t sampleR); // Used for reading a sample for storing data about waveform and peaks

    const int *getValueData();

    const int *getAudioData();

    int *getOtherData();

    int getPeakL();

    int getPeakR();

private:
    int audioDataArrayIndex = 0;
    int valueData[MAX_VALUE_COUNT]; // Data for the controlled values of the
    int audioData[AUDIO_DATA_ARRAY_LENGTH]; // Data for visualizing the waveform of the node (Can be used as double buffer)
    int *otherData; // Lets the node compile its own proprietary data for the frontend to read
    int peakL, peakR; // For getting the peak of the output to visualize output level


};


#endif //INFLUX2_AUDIONODE_H
