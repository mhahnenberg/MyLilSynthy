//
//  Envelope.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef Envelope_hpp
#define Envelope_hpp

#include <stdio.h>

class Envelope {
public:
    enum State {
        Off,
        Attack,
        Decay,
        Sustain,
        Release,
    };
    
    Envelope(float minSampleFactor, float attack, float decay, float sustain, float release);
    
    State state() const { return _state; }
    float attack() const { return _attack; }
    float decay() const { return _decay; }
    float sustain() const { return _sustain; }
    float release() const { return _release; }

    void enterState(State newState);
    void enterNextState();
    float computeNextSampleFactor();
    
private:
    float computeGainModifierFactor(float startLevel, float endLevel, u_int64_t lengthInSamples) const;

    State _state;
    float _minSampleFactor;
    float _attack;
    float _decay;
    float _sustain;
    float _release;
    float _currentGain;
    float _gainModifierFactor;
    u_int64_t _currentSampleIndex;
    u_int64_t _nextStageSampleIndex;
};

#endif /* Envelope_hpp */
