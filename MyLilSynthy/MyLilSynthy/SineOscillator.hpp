//
//  Oscillator.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/6/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include <unistd.h>
#include "AbstractOscillator.hpp"

class SineOscillator : public AbstractOscillator {
public:
    SineOscillator(int frequency);
    
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) override;

private:
    float _nextSineX;
};

inline SineOscillator::SineOscillator(int frequency)
    : AbstractOscillator(frequency)
    , _nextSineX(0.0)
{
}

#endif /* Oscillator_hpp */
