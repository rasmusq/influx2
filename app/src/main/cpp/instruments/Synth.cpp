#include <math.h>
#include "Synth.h"
#include "../Helpers.h"

void Synth::handleOutputAudio(int32_t *buffer, int32_t bufferSize) {
    for (int i = 0; i < bufferSize * 2; i += 2) {
//        double freq =
//                (sin(fmod((sampleCount * 2.0 * M_PI / 48000.0 / 8.0), (2.0 * M_PI))) + 1) * 440.0 /
//                2.0;
//        int32_t value = sin(static_cast<double>(sampleCount) * 2.0 * M_PI * freq / 48000.0) *
//                        std::numeric_limits<int32_t>::max() * amplitude;
        bool up = sampleCount % (int)(48000.0 / frequency) > (int)((48000.0 / frequency) / 2.0);
        int32_t value = 0;
        if (up) {
            value = (int) (Helpers::MAX32_AMPLITUDE);
        } else {
            value = (int) (Helpers::MIN32_AMPLITUDE);
        }
        sampleCount++;
        if (playing) {
            amplitude = amplitude < 1 ? amplitude + 0.01 : 1;
        } else {
            amplitude = amplitude > 0 ? amplitude - 0.01 : 0;
        }
        buffer[i] = value;
        buffer[i + 1] = value;
        readSample(buffer[i], buffer[i+1]);
    }
}

void Synth::handleMidi(int32_t *midiData, int32_t midiDataLength) {
    frequency = midiData[0];
    if (midiData[0] == 0) {
        playing = false;
    } else if (midiData[0] == 1) {
        playing = true;
    }
}

bool Synth::handlesOutputAudio() { return true; }

bool Synth::handlesMidi() { return true; }
