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
    SineOscillator(int frequency, float gain);
    
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) override;

private:
    float _nextSineX;
};

#endif /* Oscillator_hpp */
