//
//  SourceNodeHandler.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 18/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit
import SpriteKit

class SourceNodeHandler: NSObject {
    let node: SKNode
    
    var hasChanged: Bool = true
    
    init(node: SKNode) {
        self.node = node
    }
    
    func changePosition(newPosition: CGPoint) {
        if node.position != newPosition {
            node.position = newPosition
            hasChanged = true
        }
    }
    
    func updatePosition() {
    }
}

class TopSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        PositionSingleton.instance.setCartesianCoordinates(x: -newPosition.y, y: newPosition.x)
    }
    
    override func updatePosition() {
        let sourcePosition = PositionSingleton.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.y, -sourcePosition.x))
        
    }
}

class FrontSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        PositionSingleton.instance.setCartesianCoordinates(y: newPosition.x, z: newPosition.y)
    }
    
    override func updatePosition() {
        let sourcePosition = PositionSingleton.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.y, sourcePosition.z))
    }
}

class SideSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        PositionSingleton.instance.setCartesianCoordinates(x: newPosition.x, z: newPosition.y)
    }
    
    override func updatePosition() {
        let sourcePosition = PositionSingleton.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.x, sourcePosition.z))
    }
}
