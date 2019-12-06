//
//  Synth.cpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/5/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#include "Synth.hpp"
#include <stdio.h>
#include <limits>
#include <sys/mman.h>
#include <CoreAudio/CoreAudio.h>
#include "MyLilSynthy-Bridging-Header.h"

#define M_TAU (2 * M_PI)

const double noteFrequencies[12][9] = {
    {
        16.35,    // C0
        32.70,    // C1
        65.41,    // C2
        130.81,    // C3
        261.63,    // C4
        523.25,    // C5
        1046.50,    // C6
        2093.00,    // C7
        4186.01,    // C8
    },
    {
        17.32,    // C#0
        34.65,    // C#1
        69.30,    // C#2
        138.59,    // C#3
        277.18,    // C#4
        554.37,    // C#5
        1108.73,    // C#6
        2217.46,    // C#7
        4434.92,    // C#8
    },
    {
        18.35,    // D0
        36.71,    // D1
        73.42,    // D2
        146.83,    // D3
        293.66,    // D4
        587.33,    // D5
        1174.66,    // D6
        2349.32,    // D7
        4698.63,    // D8
    },
    {
        19.45,    // D#0
        38.89,    // D#1
        77.78,    // D#2
        155.56,    // D#3
        311.13,    // D#4
        622.25,    // D#5
        1244.51,    // D#6
        2489.02,    // D#7
        4978.03,    // D#8
    },
    {
        20.60,    // E0
        41.20,    // E1
        82.41,    // E2
        164.81,    // E3
        329.63,    // E4
        659.25,    // E5
        1318.51,    // E6
        2637.02,    // E7
        5274.04,    // E8
    },
    {
        21.83,    // F0
        43.65,    // F1
        87.31,    // F2
        174.61,    // F3
        349.23,    // F4
        698.46,    // F5
        1396.91,    // F6
        2793.83,    // F7
        5587.65,    // F8
    },
    {
        23.12,    // F#0
        46.25,    // F#1
        92.50,    // F#2
        185.00,    // F#3
        369.99,    // F#4
        739.99,    // F#5
        1479.98,    // F#6
        2959.96,    // F#7
        5919.91,    // F#8
    },
    {
        24.50,    // G0
        49.00,    // G1
        98.00,    // G2
        196.00,    // G3
        392.00,    // G4
        783.99,    // G5
        1567.98,    // G6
        3135.96,    // G7
        6271.93,    // G8
    },
    {
        25.96,    // G#0
        51.91,    // G#1
        103.83,    // G#2
        207.65,    // G#3
        415.30,    // G#4
        830.61,    // G#5
        1661.22,    // G#6
        3322.44,    // G#7
        6644.88,    // G#8
    },
    {
        27.50,    // A0
        55.00,    // A1
        110.00,    // A2
        220.00,    // A3
        440.00,    // A4
        880.00,    // A5
        1760.00,    // A6
        3520.00,    // A7
        7040.00,    // A8
    },
    {
        29.14,    // A#0
        58.27,    // A#1
        116.54,    // A#2
        233.08,    // A#3
        466.16,    // A#4
        932.33,    // A#5
        1864.66,    // A#6
        3729.31,    // A#7
        7458.62,    // A#8
    },
    {
        30.87,    // B0
        61.74,    // B1
        123.47,    // B2
        246.94,    // B3
        493.88,    // B4
        987.77,    // B5
        1975.53,    // B6
        3951.07,    // B7
        7902.13,    // B8
    },
};

void Synth::computeSineWave() {
    SoundOutputBuffer* soundBuffer = &this->_soundOutputData->soundBuffer;
    int toneHz = noteFrequencies[this->_getHighestNote()][this->_currentOctave];
    int16_t toneVolume = 3000;
    int wavePeriod = soundBuffer->samplesPerSecond / toneHz;
    
    int16_t *sampleOut = soundBuffer->samples;
    for(int SampleIndex = 0; SampleIndex < soundBuffer->sampleCount; ++SampleIndex) {
        float sineValue = sinf(soundBuffer->tSine);
        int16_t sampleValue = (int16_t)(sineValue * toneVolume);
        
        *sampleOut++ = sampleValue;
        *sampleOut++ = sampleValue;
        
        soundBuffer->tSine += M_TAU * 1.0f / (float)wavePeriod;
        if (soundBuffer->tSine > M_TAU) {
            soundBuffer->tSine -= M_TAU;
        }
    }
}

void Synth::zeroFill() {
    SoundOutputBuffer* soundBuffer = &this->_soundOutputData->soundBuffer;
    int16_t sampleValue = 0;
    int16_t *sampleOut = soundBuffer->samples;
    for(int SampleIndex = 0; SampleIndex < soundBuffer->sampleCount; ++SampleIndex) {
        *sampleOut++ = sampleValue;
        *sampleOut++ = sampleValue;
    }
}

OSStatus OSXAudioUnitCallback(void * inRefCon,
                              AudioUnitRenderActionFlags * ioActionFlags,
                              const AudioTimeStamp* inTimeStamp,
                              UInt32 inBusNumber,
                              UInt32 inNumberFrames,
                              AudioBufferList * ioData)
{
#pragma unused(ioActionFlags)
#pragma unused(inTimeStamp)
#pragma unused(inBusNumber)
    Synth* synth = ((Synth*)inRefCon);
    synth->getSoundOutputData()->soundBuffer.sampleCount = inNumberFrames;
    if (synth->isPlaying()) {
        synth->computeSineWave();
    } else {
        synth->zeroFill();
    }
    
    int16_t* outputBufferL = (int16_t *)ioData->mBuffers[0].mData;
    int16_t* outputBufferR = (int16_t *)ioData->mBuffers[1].mData;
    
    int sampleCount = synth->getSoundOutputData()->soundBuffer.sampleCount;
    int16_t* sampleBuffer = synth->getSoundOutputData()->soundBuffer.samples;
    for (uint32_t i = 0; i < sampleCount; ++i) {
        outputBufferL[i] = *sampleBuffer++;
        outputBufferR[i] = *sampleBuffer++;
    }
    
    for (uint32_t i = sampleCount; i < inNumberFrames; ++i) {
        outputBufferL[i] = 0.0;
        outputBufferR[i] = 0.0;
    }
    return noErr;
}

void Synth::initialize() {
    this->_soundOutputData = (SoundOutputData*)malloc(sizeof(SoundOutputData));
    this->_soundOutputData->soundBuffer.tSine = 0.0;
    this->_soundOutputData->soundBuffer.samplesPerSecond = 48000;
    this->_soundOutputData->soundBuffer.sampleCount = this->_soundOutputData->soundBuffer.samplesPerSecond / 60.0;
    this->_soundOutputData->soundBufferSize = this->_soundOutputData->soundBuffer.samplesPerSecond * sizeof(int16_t) * 2;
    this->_soundOutputData->soundBuffer.samples = (int16_t*)malloc(this->_soundOutputData->soundBufferSize);
    memset(this->_soundOutputData->soundBuffer.samples, 0, this->_soundOutputData->soundBufferSize);
    
    uint32_t maxPossibleOverrun = 8 * 2 * sizeof(int16_t);
    this->_soundOutputData->coreAudioBuffer = (int16_t*)mmap(0, this->_soundOutputData->soundBufferSize + maxPossibleOverrun,
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_PRIVATE | MAP_ANON,
                                                   -1,
                                                   0);
    if (this->_soundOutputData->coreAudioBuffer == MAP_FAILED) {
        printf("Core Audio Buffer mmap error: %d  %s", errno, strerror(errno));
    }
    memset(this->_soundOutputData->coreAudioBuffer, 0, this->_soundOutputData->soundBufferSize);
    
    printf("Initializing CoreAudio...");
    AudioComponentDescription acd;
    acd.componentType         = kAudioUnitType_Output;
    acd.componentSubType      = kAudioUnitSubType_DefaultOutput;
    acd.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    AudioComponent outputComponent = AudioComponentFindNext(NULL, &acd);
    
    AudioComponentInstanceNew(outputComponent, &this->_soundOutputData->audioUnit);
    AudioUnitInitialize(this->_soundOutputData->audioUnit);
    
    // uint16
    //AudioStreamBasicDescription asbd;
    this->_soundOutputData->audioDescriptor.mSampleRate       = this->_soundOutputData->soundBuffer.samplesPerSecond;
    this->_soundOutputData->audioDescriptor.mFormatID         = kAudioFormatLinearPCM;
    this->_soundOutputData->audioDescriptor.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsNonInterleaved | kAudioFormatFlagIsPacked;
    this->_soundOutputData->audioDescriptor.mFramesPerPacket  = 1;
    this->_soundOutputData->audioDescriptor.mChannelsPerFrame = 2; // Stereo
    this->_soundOutputData->audioDescriptor.mBitsPerChannel   = sizeof(int16_t) * 8;
    this->_soundOutputData->audioDescriptor.mBytesPerFrame    = sizeof(int16_t); // don't multiply by channel count with non-interleaved!
    this->_soundOutputData->audioDescriptor.mBytesPerPacket   = this->_soundOutputData->audioDescriptor.mFramesPerPacket * this->_soundOutputData->audioDescriptor.mBytesPerFrame;
    
    
    
    AudioUnitSetProperty(this->_soundOutputData->audioUnit,
                         kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Input,
                         0,
                         &this->_soundOutputData->audioDescriptor,
                         sizeof(this->_soundOutputData->audioDescriptor));
    
    AURenderCallbackStruct cb;
    cb.inputProc = OSXAudioUnitCallback;
    cb.inputProcRefCon = this;
    
    AudioUnitSetProperty(this->_soundOutputData->audioUnit,
                         kAudioUnitProperty_SetRenderCallback,
                         kAudioUnitScope_Global,
                         0,
                         &cb,
                         sizeof(cb));
    
    AudioOutputUnitStart(this->_soundOutputData->audioUnit);
    printf("Done.\n");
}

Note Synth::_getHighestNote() {
    Note maxNote = *this->_activeNotes.begin();
    for (auto note : this->_activeNotes) {
        if (note > maxNote) {
            maxNote = note;
        }
    }
    return maxNote;
}

void Synth::startPlayingNote(Note note) {
    this->_activeNotes.insert(note);
    this->_isPlaying = true;
    printf("enabling playback\n");
}

void Synth::stopPlayingNote(Note note) {
    if (this->_activeNotes.size() == 1) {
        printf("activeNotes will be empty, disabling playback\n");
        this->_isPlaying = false;
    }
    this->_activeNotes.erase(note);
}

void Synth::tryIncrementOctave() {
    if (this->_currentOctave == this->_maxOctave) {
        return;
    }
    this->_currentOctave++;
}

void Synth::tryDecrementOctave() {
    if (this->_currentOctave == 0) {
        return;
    }
    this->_currentOctave--;
}

extern "C" {
const void *createSynth(int maxOctave) {
    Synth* synth = new Synth(maxOctave);
    return (void *)synth;
}

void initializeSynth(const void *object) {
    Synth* synth = (Synth *)object;
    synth->initialize();
}

void tryIncrementOctave(const void *object) {
    Synth* synth = (Synth *)object;
    synth->tryIncrementOctave();
}

void tryDecrementOctave(const void *object) {
    Synth* synth = (Synth *)object;
    synth->tryDecrementOctave();
}

void startPlayingNote(const void* object, Note note) {
    Synth* synth = (Synth *)object;
    synth->startPlayingNote(note);
}

void stopPlayingNote(const void* object, Note note) {
    Synth* synth = (Synth *)object;
    synth->stopPlayingNote(note);
}
}
