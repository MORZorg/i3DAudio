//
//  HeadNodeHandler.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 21/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit
import SpriteKit

class HeadNodeHandler: NSObject {
    let node: SKNode
    
    var hasChanged: Bool = true
    
    init(node: SKNode) {
        self.node = node
    }
    
    func changeRotation(newRotation: CGFloat) {
        if node.zRotation != newRotation {
            node.zRotation = newRotation
            hasChanged = true
        }
    }
    
    func updateRotation() {
    }
    
    func getTexture() -> SKTexture? {
        return nil
    }
}

class TopHeadNodeHandler: HeadNodeHandler {
    override func changeRotation(newRotation: CGFloat) {
        super.changeRotation(newRotation)
        HeadOrientation.instance.setYawRadians(newRotation)
    }
    
    override func updateRotation() {
        super.changeRotation(HeadOrientation.instance.getYawRadians())
    }
    
    override func getTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadTop")
    }
}

class FrontHeadNodeHandler: HeadNodeHandler {
    override func changeRotation(newRotation: CGFloat) {
        super.changeRotation(newRotation)
        HeadOrientation.instance.setRollRadians(newRotation)
    }
    
    override func updateRotation() {
        super.changeRotation(HeadOrientation.instance.getRollRadians())
    }
    
    override func getTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadFront")
    }
}

class SideHeadNodeHandler: HeadNodeHandler {
    override func changeRotation(newRotation: CGFloat) {
        super.changeRotation(newRotation)
        HeadOrientation.instance.setPitchRadians(newRotation)
    }
    
    override func updateRotation() {
        super.changeRotation(HeadOrientation.instance.getPitchRadians())
    }
    
    override func getTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadSide")
    }
}
