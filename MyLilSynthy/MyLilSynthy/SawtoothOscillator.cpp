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

SawtoothOscillator::SawtoothOscillator(int frequency, float gain)
    : AbstractOscillator(frequency, gain)
{
    int samplesPerSecond = 48000;
    int numOscillators = (samplesPerSecond / 2) / frequency;
    for (int i = 1; i <= numOscillators; ++i) {
        float gain = 1.0 / (float)i;
        this->_oscillators.push_back(std::make_unique<SineOscillator>(frequency * i, gain));
    }
}

SawtoothOscillator::~SawtoothOscillator()
{
}

void SawtoothOscillator::computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) {
    float privateSampleBuffer[sampleCount * 2];
    memset(privateSampleBuffer, 0, sizeof(float) * sampleCount * 2);
    // First compute the raw values for each of the component sine oscillators.
    for (auto& oscillator : this->_oscillators) {
        oscillator->computeSamples(privateSampleBuffer, sampleCount, samplesPerSecond);
    }
    
    bool softStart = this->_softStart;
    bool softStop = this->_softStop;
    float gain = this->gain();
    
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
        float sampleValue = privateSampleBuffer[sampleIndex * 2] * currSignalDamping * gain;

        float remainingSignalDamping = targetSignalDamping - currSignalDamping;
        if (remainingSignalDamping <= -0.025 || remainingSignalDamping >= 0.025) {
            currSignalDamping += signalDampingStep;
        }
        
        *sampleOut++ += sampleValue;
        *sampleOut++ += sampleValue;
    }
    
    if (softStop) {
        this->_isPlaying = false;
    }
    this->_softStart = false;
    this->_softStop = false;
}
