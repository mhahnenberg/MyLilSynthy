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

class SineOscillator {
public:
    SineOscillator(int frequency, int volume)
        : _frequency(frequency)
        , _nextSineX(0.0)
        , _isPlaying(false)
        , _softStart(false)
        , _softStop(false)
    {
    }
    
    int frequency() const { return this->_frequency; }
    void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond);
    void start();
    void stop();
    bool isPlaying() const { return this->_isPlaying; }

private:
    int _frequency;
    float _nextSineX;
    bool _isPlaying;
    bool _softStart;
    bool _softStop;
};

#endif /* Oscillator_hpp */
