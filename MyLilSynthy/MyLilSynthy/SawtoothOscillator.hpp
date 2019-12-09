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
#include <vector>
#include "AbstractOscillator.hpp"
#include "SineOscillator.hpp"

class SawtoothOscillator : public AbstractOscillator {
public:
    SawtoothOscillator(int frequency, float gain);
    virtual ~SawtoothOscillator() override;
    
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) override;
    
private:
    std::vector<std::unique_ptr<SineOscillator>> _oscillators;
    
    const static int s_numSineOscillators = 200;
};

#endif /* SawtoothOscillator_hpp */
