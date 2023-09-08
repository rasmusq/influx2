#ifndef INFLUX2_SPRING_H
#define INFLUX2_SPRING_H


#include "../AudioNode.h"

class Spring : public AudioNode {

    static const int SPRING_RES = 64;
    float springArrayPositionL[SPRING_RES];
    float springArrayPositionR[SPRING_RES];
    float springArrayVelocityL[SPRING_RES];
    float springArrayVelocityR[SPRING_RES];
    float dampeningL = 0.99985;
    float tensionL = 0.00700;
    float dampeningR = 0.99985;
    float tensionR = 0.00625;
    float wetness = 0.8;

public:
    void handleOutputAudio(int32_t *buffer, int32_t bufferSize) override;

    bool handlesOutputAudio() override;
};


#endif //INFLUX2_SPRING_H
