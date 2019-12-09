//
//  SawtoothOscillator.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef SawtoothOscillator_hpp
#define SawtoothOscillator_hpp

#include <stdio.h>
#include "AbstractOscillator.hpp"

class SawtoothOscillator : public AbstractOscillator {
public:
    SawtoothOscillator(int frequency)
        : AbstractOscillator(frequency)
        , _nextSineX(0.0)
    {
    }
    
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) override;
    
private:
    float _nextSineX;
};

#endif /* SawtoothOscillator_hpp */
