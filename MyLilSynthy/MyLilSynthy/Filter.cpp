//
//  Filter.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/10/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include "Filter.hpp"

Filter::Filter()
    : _mode(Lowpass)
    , _cutoff(0.999)
    , _resonance(0.0)
    , _buffer0(0.0)
    , _buffer1(0.0)
    , _buffer2(0.0)
    , _buffer3(0.0)
    , _feedback(this->computeFeedback())
{
}

float Filter::computeFeedback()
{
    return this->_resonance + this->_resonance/(1.0 - this->_cutoff);
}

// Algorithm shamelessly borrowed from http://www.martin-finke.de/blog/articles/audio-plugins-013-filter/
float Filter::processSample(float sample)
{
    this->_buffer0 += this->_cutoff * (sample - this->_buffer0 + this->_feedback * (this->_buffer0 - this->_buffer1));
    this->_buffer1 += this->_cutoff * (this->_buffer0 - this->_buffer1);
    this->_buffer2 += this->_cutoff * (this->_buffer1 - this->_buffer2);
    this->_buffer3 += this->_cutoff * (this->_buffer2 - this->_buffer3);

    switch (this->_mode) {
        case Lowpass:
            return this->_buffer3;
        case Highpass:
            return sample - this->_buffer3;
        case Bandpass:
            return this->_buffer0 - this->_buffer3;
        default:
            return 0.0;
    }
}
