//
//  SourceNodeHandler.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 18/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit
import SpriteKit

class SourceNodeHandler {
    let sourceNode: SKNode
    
    var hasChanged: Bool = true
    
    init(node: SKNode) {
        sourceNode = node
    }
    
    func changePosition(newPosition: CGPoint) {
        if sourceNode.position != newPosition {
            sourceNode.position = newPosition
            hasChanged = true
        }
    }
    
    func updatePosition() {
        
    }
    
    func getHeadTexture() -> SKTexture? {
        return nil
    }
}

class TopSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        SourceData.instance.setCartesianCoordinates(x: -newPosition.y, y: newPosition.x)
    }
    
    override func updatePosition() {
        let sourcePosition = SourceData.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.y, -sourcePosition.x))
    }
    
    override func getHeadTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadTop")
    }
}

class FrontSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        SourceData.instance.setCartesianCoordinates(y: newPosition.x, z: newPosition.y)
    }
    
    override func updatePosition() {
        let sourcePosition = SourceData.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.y, sourcePosition.z))
    }
    
    override func getHeadTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadFront")
    }
}

class SideSourceNodeHandler: SourceNodeHandler {
    override func changePosition(newPosition: CGPoint) {
        super.changePosition(newPosition)
        SourceData.instance.setCartesianCoordinates(x: newPosition.x, z: newPosition.y)
    }
    
    override func updatePosition() {
        let sourcePosition = SourceData.instance.getCartesianCoordinates()
        super.changePosition(CGPointMake(sourcePosition.x, sourcePosition.z))
    }
    
    override func getHeadTexture() -> SKTexture? {
        return SKTexture(imageNamed: "HeadSide")
    }
}
