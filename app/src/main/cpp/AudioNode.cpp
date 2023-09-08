#include "AudioNode.h"

void AudioNode::readSample(int32_t sample) {
   peakValue = sample;
   audioData[audioDataArrayIndex] = sample;
   audioDataArrayIndex++;
   if(audioDataArrayIndex >= AUDIO_DATA_ARRAY_LENGTH) {
       audioDataArrayIndex = 0;
   }

}