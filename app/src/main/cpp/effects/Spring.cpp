#include "Spring.h"

void Spring::handleOutputAudio(int32_t *buffer, int32_t bufferSize) {
    for(int i = 0; i < bufferSize*2; i += 2) {
        springArrayPositionL[0] = static_cast<float>(buffer[i]);
        springArrayPositionR[0] = static_cast<float>(buffer[i+1]);
        for(int j = 1; j < SPRING_RES-1; j++) {
            springArrayVelocityL[j] += ((springArrayPositionL[j - 1] - springArrayPositionL[j]) + (springArrayPositionL[j + 1] - springArrayPositionL[j])) * tensionL;
            springArrayVelocityR[j] += ((springArrayPositionR[j - 1] - springArrayPositionR[j]) + (springArrayPositionR[j + 1] - springArrayPositionR[j])) * tensionR;
            springArrayVelocityL[j] *= dampeningL;
            springArrayVelocityR[j] *= dampeningR;
        }
        for(int j = 1; j < SPRING_RES-1; j++) {
            springArrayPositionL[j] += springArrayVelocityL[j];
            springArrayPositionR[j] += springArrayVelocityR[j];
        }
        buffer[i] = springArrayPositionL[SPRING_RES / 2] * wetness + buffer[i] * (1 - wetness);
        buffer[i+1] = springArrayPositionR[SPRING_RES / 2] * wetness + buffer[i + 1] * (1 - wetness);
    }
}

bool Spring::handlesOutputAudio() { return true; }