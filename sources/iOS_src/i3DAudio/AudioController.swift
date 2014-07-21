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
        OrzHrtfTildeCompatibility.setup()
        
        PdBase.openFile("hrtf.pd", path: NSBundle.mainBundle().resourcePath)
        controller.active = true
        
        controller.print()
        
        // This bang seems useless...
        PdBase.sendBangToReceiver("open")
        PdBase.sendBangToReceiver("start")
        
        SourcePosition.instance.delegate = self
        HeadOrientation.instance.delegate = self
    }
    
    func receivePrint(message: String) {
        println("(pd) \(message)");
    }
    
    func update(object: AnyObject, property: String) {
        PdBase.sendFloat(object.valueForKey(property) as Float, toReceiver: property)
    }
}