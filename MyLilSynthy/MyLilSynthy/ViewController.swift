//
//  ViewController.swift
//  MyLilSynthy
//
//  Created by Mark Hahnenberg on 12/5/19.
//  Copyright © 2019 Mark Hahnenberg. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {
    var synth: UnsafeRawPointer;
    
    required init?(coder: NSCoder) {
        let maxOctave: Int32 = 8
        self.synth = createSynth(maxOctave)!
        
        super.init(coder: coder)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        
        initializeSynth(self.synth)
        
        NSEvent.addLocalMonitorForEvents(matching: .keyUp) { (aEvent) -> NSEvent? in
            self.keyUp(with: aEvent)
            if (self.canHandleKeyDown(with: aEvent)) {
                return nil;
            }
            return aEvent
        }

        NSEvent.addLocalMonitorForEvents(matching: .keyDown) { (aEvent) -> NSEvent? in
            self.keyDown(with: aEvent)
            if (self.canHandleKeyDown(with: aEvent)) {
                return nil;
            }
            return aEvent
        }
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }

    override var acceptsFirstResponder: Bool { return true }
    override func becomeFirstResponder() -> Bool { return true }
    override func resignFirstResponder() -> Bool { return true }

    func canHandleKeyDown(with event: NSEvent) -> Bool {
        return self.octaveStepForKeyCode(keyCode: event.keyCode) != nil || noteForKeyCode(keyCode: event.keyCode) != nil
    }
    
    func noteForKeyCode(keyCode: UInt16) -> Note? {
        switch keyCode {
        case 0:
            return C;
        case 13:
            return CSharp;
        case 1:
            return D;
        case 14:
            return DSharp;
        case 2:
            return E;
        case 3:
            return F;
        case 17:
            return FSharp;
        case 5:
            return G;
        case 16:
            return GSharp;
        case 4:
            return A;
        case 32:
            return ASharp;
        case 38:
            return B;
        default:
            print(String(format:"keycode: %d", keyCode))
            return nil
        }
    }
    
    func octaveStepForKeyCode(keyCode: UInt16) -> Int? {
        switch keyCode {
        case 6:
            return -1
        case 7:
            return 1
        default:
            return nil
        }
    }
    
    override func keyDown(with event: NSEvent) {
        let octaveStep = self.octaveStepForKeyCode(keyCode: event.keyCode)
        if octaveStep != nil {
            if octaveStep! > 0 {
                tryIncrementOctave(self.synth)
            } else {
                tryDecrementOctave(self.synth)
            }
            return;
        }
        
        let note = self.noteForKeyCode(keyCode: event.keyCode)
        if note == nil {
            return;
        }
        startPlayingNote(self.synth, note!)
     }
        
    override func keyUp(with event: NSEvent) {
        let note = self.noteForKeyCode(keyCode: event.keyCode)
        if note == nil {
            return;
        }
        stopPlayingNote(self.synth, note!)
    }
}

