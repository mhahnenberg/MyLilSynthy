//
//  Filter.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/10/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef Filter_hpp
#define Filter_hpp

#include <stdio.h>

class Filter {
public:
    enum Mode {
        Lowpass,
        Highpass,
        Bandpass,
    };
    
    Filter();
    
    Mode mode() const { return this->_mode; }
    float cutoff() const { return this->_cutoff; }
    float resonance() const { return this->_resonance; }
    float feedback() const { return this->_feedback; }
    
    void setMode(Mode mode) { this->_mode = mode; }
    void setResonance(float resonance) { this->_resonance = resonance; }
    void setCutoff(float cutoff) { this->_cutoff = cutoff; }
    
    float processSample(float sample);
    
private:
    float computeFeedback();
    
    Mode _mode;
    float _cutoff;
    float _resonance;
    float _feedback;
    float _buffer0;
    float _buffer1;
    float _buffer2;
    float _buffer3;
};

#endif /* Filter_hpp */
