//
//  Oscillator.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include "Oscillator.hpp"
#include <math.h>

#define M_TAU (2 * M_PI)

Oscillator::Oscillator(OscillatorType type, int frequency, float gain)
    : _type(type)
    , _frequency(frequency)
    , _gain(gain)
    , _envelope(0.0001, 0.01, 0.5, 0.1, 1.0)
    , _filter()
    , _nextX(0.0)
{
}

float Oscillator::computeSampleValue(float x) {
    switch (this->_type) {
        case Sine:
            return sinf(x);
        case Saw:
            return 1.0 - (2.0 * x /  M_TAU);
        case Square:
            return x <= M_PI ? 1.0 : -1.0;
        case Triangle:
            float rawValue = -1.0 + (2.0 * x / M_TAU);
            return 2.0 * (fabs(rawValue) - 0.5);
    }
}

void Oscillator::computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) {
    int wavePeriod = samplesPerSecond / this->frequency();
    float gain = this->gain();
    
    float *sampleOut = sampleBuffer;
    for (int sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
        float sampleValue = this->computeSampleValue(this->_nextX);
        sampleValue *= this->_envelope.computeNextSampleFactor();
        sampleValue *= gain;
        sampleValue = this->_filter.processSample(sampleValue);
        
        *sampleOut++ += sampleValue;
        *sampleOut++ += sampleValue;
        
        this->_nextX += M_TAU * 1.0f / (float)wavePeriod;
        if (this->_nextX > M_TAU) {
            this->_nextX -= M_TAU;
        }
    }
}
