//
//  Oscillator.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include <stdio.h>

class Oscillator {
public:
    enum OscillatorType {
        Sine,
        Saw,
        Square,
        Triangle,
    };
    
    Oscillator(OscillatorType type, int frequency, float gain);
    
    int frequency() const;
    float gain() const;
    OscillatorType type() const;
    bool isPlaying() const;
    void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond);
    void start();
    void stop();
    
private:
    float computeSampleValue(float x);
    
    OscillatorType _type;
    int _frequency;
    float _gain;
    bool _isPlaying;
    bool _softStart;
    bool _softStop;
    
    float _nextX;
};

inline Oscillator::OscillatorType Oscillator::type() const {
    return this->_type;
}

inline int Oscillator::frequency() const {
    return this->_frequency;
}

inline float Oscillator::gain() const {
    return this->_gain;
}

inline bool Oscillator::isPlaying() const {
    return this->_isPlaying;
}

inline void Oscillator::start() {
    this->_softStart = true;
    this->_isPlaying = true;
}

inline void Oscillator::stop() {
    this->_softStop = true;
}

#endif /* Oscillator_hpp */
