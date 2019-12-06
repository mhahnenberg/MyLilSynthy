//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//


#ifndef Synth_hpp
#define Synth_hpp

#ifdef __cplusplus
extern "C" {
#endif
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

    const void *createSynth(int maxOctave);
    void initializeSynth(const void *object);
    void tryIncrementOctave(const void *object);
    void tryDecrementOctave(const void *object);
    void startPlayingNote(const void* object, Note);
    void stopPlayingNote(const void* object, Note);

#ifdef __cplusplus
}
#endif

#endif /* Synth_hpp */
