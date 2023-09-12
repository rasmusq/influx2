#include "AudioNode.h"

void AudioNode::readSample(int32_t sampleL, int32_t sampleR) {
   peakL = sampleL;
   peakR = sampleR;
   audioData[audioDataArrayIndex] = sampleL;
   audioDataArrayIndex++;
   audioData[audioDataArrayIndex] = sampleR;
   audioDataArrayIndex++;
   if(audioDataArrayIndex >= AUDIO_DATA_ARRAY_LENGTH) {
       audioDataArrayIndex = 0;
   }

}

const int *AudioNode::getValueData() {
    return valueData;
}

const int *AudioNode::getAudioData() {
    return audioData;
}

int *AudioNode::getOtherData() {
    return otherData;
}

int AudioNode::getPeakL() {
    return peakL;
}

int AudioNode::getPeakR() {
    return peakR;
}
