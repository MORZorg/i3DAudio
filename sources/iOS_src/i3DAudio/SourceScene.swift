//
//  GameScene.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 13/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import SpriteKit

extension SKNode {
    func bearingTowards(node: SKNode) -> Float {
        return atan2(node.position.y - position.y, node.position.x - position.x)
    }
}

class SourceScene: SKScene {
    let perspectiveImages = ["side": "HeadSide", "front": "HeadFront"]
        
    var shownPerspective: String?
    var headNode: SKNode?
    var sourceNode: SKNode?
    var currentTouch: AnyObject?
    
    override func didMoveToView(view: SKView) {
        headNode = self.childNodeWithName("head")
        sourceNode = self.childNodeWithName("source")
        
        headNode!.runAction(SKAction.setTexture(SKTexture(imageNamed: perspectiveImages[shownPerspective!])))
        
        physicsBody = SKPhysicsBody(edgeLoopFromRect: frame)
    }
    
    override func touchesBegan(touches: NSSet, withEvent event: UIEvent) {
        /* Called when a touch begins */
//        for touch in touches {
//            if sourceNode == nodeAtPoint(touch.locationInNode(self)) {
//                currentTouch = touch
//            }
//        }
        currentTouch = touches.anyObject()
    }
    
    override func touchesMoved(touches: NSSet!, withEvent event: UIEvent!) {
        /* Called when a touch moves */
        if let location = currentTouch?.locationInNode(self) {
            sourceNode!.position = location
            
            switch(shownPerspective!) {
            case "side":
                SourceData.instance.setCartesianCoordinates(x: Double(location.x), z: Double(location.y))
                
            case "front":
                SourceData.instance.setCartesianCoordinates(y: Double(location.x), z: Double(location.y))
                
            default:
                break
            }
        }
    }
    
    override func touchesEnded(touches: NSSet!, withEvent event: UIEvent!) {
        /* Called when a touch ends */
        if let location = currentTouch?.locationInNode(self) {
            sourceNode!.position = location
            
            switch(shownPerspective!) {
            case "side":
                SourceData.instance.setCartesianCoordinates(x: Double(location.x), z: Double(location.y))
                
            case "front":
                SourceData.instance.setCartesianCoordinates(y: Double(location.x), z: Double(location.y))
                
            default:
                break
            }
        }
        currentTouch = nil
    }
   
    override func update(currentTime: CFTimeInterval) {
        /* Called before each frame is rendered */
        let coordinates = SourceData.instance.getCartesianCoordinates()
        switch(shownPerspective!) {
        case "side":
            sourceNode!.position = CGPointMake(Float(coordinates.x), Float(coordinates.z))
            
        case "front":
            sourceNode!.position = CGPointMake(Float(coordinates.y), Float(coordinates.z))
            
        default:
            break
        }
        
        sourceNode!.zRotation = sourceNode!.bearingTowards(headNode!)
    }
}
