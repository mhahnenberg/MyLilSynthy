//
//  SawtoothOscillator.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include "SawtoothOscillator.hpp"
#include <limits>
#include <math.h>

#define M_TAU (2 * M_PI)

void SawtoothOscillator::computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) {
    int frequency = this->frequency();
    int wavePeriod = samplesPerSecond / frequency;
    
    bool softStart = this->_softStart;
    bool softStop = this->_softStop;
    
    float targetSignalDamping = softStop ? 0.0 : 1.0;
    float currSignalDamping = softStart ? 0.0 : 1.0;
    float signalDampingStep = 0;
    if (softStart) {
        signalDampingStep = 0.025;
    } else if (softStop) {
        signalDampingStep = -0.025;
    }
    
    float *sampleOut = sampleBuffer;
    for (int sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
        float sineValue = sinf(this->_nextSineX) * currSignalDamping;

        float remainingSignalDamping = targetSignalDamping - currSignalDamping;
        if (remainingSignalDamping <= -0.025 || remainingSignalDamping >= 0.025) {
            currSignalDamping += signalDampingStep;
        }
        
        *sampleOut++ += sineValue;
        *sampleOut++ += sineValue;
        
        this->_nextSineX += M_TAU * 1.0f / (float)wavePeriod;
        if (this->_nextSineX > M_TAU) {
            this->_nextSineX -= M_TAU;
        }
    }
    
    if (softStop) {
        this->_isPlaying = false;
    }
    this->_softStart = false;
    this->_softStop = false;
}
