//
//  AudioController.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 18/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit

class AudioController: NSObject, SimpleObserver {
    let controller = PdAudioController()
    
    override init() {
        super.init()
        
        controller.configureAmbientWithSampleRate(44100, numberChannels: 2, mixingEnabled: true)
        PdBase.setDelegate(self)
        OrzHrtfTildeCompatibility.setup()
        
        PdBase.openFile("hrtf_stereo.pd", path: NSBundle.mainBundle().resourcePath)
        controller.active = true
        
        if DEBUG == 1 {
            controller.print()
        }
        
        PositionSingleton.instance.delegate = self
        OrientationSingleton.instance.delegate = self
    }
    
    func receivePrint(message: String) {
        println("(pd) \(message)");
    }
    
    func update(object: AnyObject, property: String) {
        PdBase.sendFloat(object.valueForKey(property) as Float, toReceiver: property)
    }
    
    func openFile(filename: NSURL) {
        PdBase.sendSymbol(filename.absoluteString, toReceiver: "open")
        PdBase.sendBangToReceiver("start")
    }
    
    func openDefault() {
        openFile(NSURL(string: "the_lord_has_spoken.wav")!)
    }
}