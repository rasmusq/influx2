#include "Spring.h"
#include "chrono"

void Spring::handleOutputAudio(int32_t *buffer, int32_t bufferSize) {
    startTime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < bufferSize*2; i += 2) {
        springArrayPositionL[0] = static_cast<float>(buffer[i]);
        springArrayPositionR[0] = static_cast<float>(buffer[i+1]);
        for(int j = 1; j < SPRING_RES-1; j++) {
            springArrayVelocityL[j] += ((springArrayPositionL[j - 1] - springArrayPositionL[j]) + (springArrayPositionL[j + 1] - springArrayPositionL[j])) * tensionL;
            springArrayVelocityR[j] += ((springArrayPositionR[j - 1] - springArrayPositionR[j]) + (springArrayPositionR[j + 1] - springArrayPositionR[j])) * tensionR;
            springArrayVelocityL[j] *= dampeningL;
            springArrayVelocityR[j] *= dampeningR;
        }
        output[i] = 0;
        output[i+1] = 0;
        for(int j = 1; j < SPRING_RES-1; j++) {
            springArrayPositionL[j] += springArrayVelocityL[j];
            springArrayPositionR[j] += springArrayVelocityR[j];
            output[i] += springArrayVelocityL[j];
            output[i+1] += springArrayVelocityR[j];
        }
        buffer[i] = (output[i] * wetness + buffer[i] * (1 - wetness)) * gain;
        buffer[i+1] = (output[i+1] * wetness + buffer[i + 1] * (1 - wetness)) * gain;
    }
    endTime = std::chrono::high_resolution_clock::now();
    if(bufferCount % 200 == 0) {
        __android_log_print(ANDROID_LOG_VERBOSE, __FUNCTION__,
                            "This buffer took: %lld nanoseconds", (static_cast<int64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count())));
    }
    bufferCount++;
}

bool Spring::handlesOutputAudio() { return true; }