//
//  AbstractOscillator.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/9/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include <stdio.h>
#include "AbstractOscillator.hpp"

AbstractOscillator::AbstractOscillator(int frequency)
    : _frequency(frequency)
    , _isPlaying(false)
    , _softStart(false)
    , _softStop(false)
{
}

AbstractOscillator::~AbstractOscillator() {
}
