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
    
    init() {
        super.init()
        
        controller.configureAmbientWithSampleRate(44100, numberChannels: 2, mixingEnabled: true)
        PdBase.setDelegate(self)
        
        PdBase.openFile("audio_test.pd", path: NSBundle.mainBundle().resourcePath)
        controller.active = true
        
        controller.print()
        
        SourceData.instance.delegate = self
    }
    
    func receivePrint(message: String) {
        println("(pd) \(message)");
    }
    
    func update(object: AnyObject, property: String) {
        // FIXME Use something like object.propertyForKey(property) as Float
        PdBase.sendFloat(0.0, toReceiver: property)
    }
}