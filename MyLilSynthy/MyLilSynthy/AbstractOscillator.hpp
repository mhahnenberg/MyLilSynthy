//
//  AbstractOscillator.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef AbstractOscillator_hpp
#define AbstractOscillator_hpp

class AbstractOscillator {
public:
    AbstractOscillator(int frequency, float gain);
    virtual ~AbstractOscillator();
    
    int frequency() const;
    float gain() const;
    bool isPlaying() const;
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) = 0;
    void start();
    void stop();
    
protected:
    int _frequency;
    float _gain;
    bool _isPlaying;
    bool _softStart;
    bool _softStop;
};

inline int AbstractOscillator::frequency() const {
    return this->_frequency;
}

inline float AbstractOscillator::gain() const {
    return this->_gain;
}

inline bool AbstractOscillator::isPlaying() const {
    return this->_isPlaying;
}

inline void AbstractOscillator::start() {
    this->_softStart = true;
    this->_isPlaying = true;
}

inline void AbstractOscillator::stop() {
    this->_softStop = true;
}

#endif /* AbstractOscillator_hpp */
