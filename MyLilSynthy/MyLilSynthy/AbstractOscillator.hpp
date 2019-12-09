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
    AbstractOscillator(int frequency);
    virtual ~AbstractOscillator();
    
    int frequency() const;
    virtual void computeSamples(float* sampleBuffer, int sampleCount, int samplesPerSecond) = 0;
    void start();
    void stop();
    bool isPlaying() const;
    
protected:
    int _frequency;
    bool _isPlaying;
    bool _softStart;
    bool _softStop;
};

inline int AbstractOscillator::frequency() const {
    return this->_frequency;
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
