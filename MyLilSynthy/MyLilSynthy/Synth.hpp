//
//  Synth.hpp
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/5/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

#ifndef Synth_hpp
#define Synth_hpp

#include <stdio.h>
#include <limits>
#include <set>
#include <unistd.h>
#include <AudioUnit/AudioUnit.h>

typedef enum {
    C = 0,
    CSharp = 1,
    D = 2,
    DSharp = 3,
    E = 4,
    F = 5,
    FSharp = 6,
    G = 7,
    GSharp = 8,
    A = 9,
    ASharp = 10,
    B = 11,
} Note;


typedef struct SoundOutputBuffer {
    float tSine;
    int samplesPerSecond;
    int sampleCount;
    int16_t* samples;
} SoundOutputBuffer;


typedef struct SoundOutputData {
    SoundOutputBuffer soundBuffer;
    uint32_t soundBufferSize;
    int16_t* coreAudioBuffer;
    AudioStreamBasicDescription audioDescriptor;
    AudioUnit audioUnit;
} SoundOutputData;

class Synth {
public:
    Synth(int maxOctave)
        : _isPlaying(false)
        , _softStart(false)
        , _softStop(false)
        , _currentOctave(4)
        , _maxOctave(maxOctave)
        , _soundOutputData(nullptr)
    {
    }

    void initialize();
    void startPlayingNote(Note);
    void stopPlayingNote(Note);
    void tryIncrementOctave();
    void tryDecrementOctave();
    SoundOutputData* getSoundOutputData() { return this->_soundOutputData; }
    bool isPlaying() const { return _isPlaying; }
    
    void computeSineWave();
    void zeroFill();

private:
    Note _getHighestNote();
    
    bool _isPlaying;
    bool _softStart;
    bool _softStop;
    int _currentOctave;
    int _maxOctave;
    std::set<Note> _activeNotes;
    SoundOutputData* _soundOutputData;
};

#endif /* Synth_hpp */
