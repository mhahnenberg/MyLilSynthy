//
//  Envelope.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include "Envelope.hpp"
#include <math.h>
#include "Constants.hpp"

Envelope::Envelope(float minSampleFactor, float attack, float decay, float sustain, float release)
    : _state(Off)
    , _minSampleFactor(minSampleFactor)
    , _attack(attack)
    , _decay(decay)
    , _sustain(sustain)
    , _release(release)
    , _currentGain(0.0)
    , _gainModifierFactor(0.0)
    , _currentSampleIndex(0)
    , _nextStageSampleIndex(0)
{
}

void Envelope::enterState(State newState)
{
    this->_state = newState;
    this->_currentSampleIndex = 0;
    switch (newState) {
        case Off:
            this->_nextStageSampleIndex = 0;
            this->_currentGain = 0.0;
            this->_gainModifierFactor = 1.0;
            break;
        case Attack:
            this->_nextStageSampleIndex = this->_attack * SAMPLE_RATE;
            this->_currentGain = this->_minSampleFactor;
            this->_gainModifierFactor = this->computeGainModifierFactor(this->_currentGain, 1.0, this->_nextStageSampleIndex);
            break;
        case Decay:
            this->_nextStageSampleIndex = this->_decay * SAMPLE_RATE;
            this->_currentGain = 1.0;
            this->_gainModifierFactor = this->computeGainModifierFactor(this->_currentGain, fmax(this->_sustain, this->_minSampleFactor), this->_nextStageSampleIndex);
            break;
        case Sustain:
            this->_nextStageSampleIndex = 0;
            this->_currentGain = _sustain;
            this->_gainModifierFactor = 1.0;
            break;
        case Release:
            this->_nextStageSampleIndex = this->_release * SAMPLE_RATE;
            // Don't change _currentGain since we may not be releasing from Sustain.
            this->_gainModifierFactor = this->computeGainModifierFactor(this->_currentGain, this->_minSampleFactor, this->_nextStageSampleIndex);
            break;
    }
}

void Envelope::enterNextState()
{
    switch (this->_state) {
        case Off:
            return enterState(Attack);
        case Attack:
            return enterState(Decay);
        case Decay:
            return enterState(Sustain);
        case Sustain:
            return enterState(Release);
        case Release:
            return enterState(Off);
    }
}

float Envelope::computeNextSampleFactor()
{
    if (this->state() == Off || this->state() == Sustain) {
        return _currentGain;
    }
    
    if (_currentSampleIndex == _nextStageSampleIndex) {
        enterNextState();
    }
    
    _currentGain *= _gainModifierFactor;
    _currentSampleIndex += 1;
    return _currentGain;
}

float Envelope::computeGainModifierFactor(float startLevel, float endLevel, u_int64_t lengthInSamples) const
{
    return 1.0 + (log(endLevel) - log(startLevel)) / (lengthInSamples);
}
