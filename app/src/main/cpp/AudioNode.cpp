#include "AudioNode.h"

void AudioNode::readSample(int32_t sample) {
   peakValue = sample;
   audioData[audioDataArrayIndex] = sample;
   audioDataArrayIndex++;

}