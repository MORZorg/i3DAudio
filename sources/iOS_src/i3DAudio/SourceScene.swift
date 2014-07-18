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
    let perspectiveImages = ["side": "HeadSide", "front": "HeadFront", "top": "HeadTop"]
    
    var shownPerspective: String?
    
    var headNode: SKNode?
    var sourceNodeHandler: SourceNodeHandler?
    var currentTouch: AnyObject?
    
    override func didMoveToView(view: SKView) {
        headNode = self.childNodeWithName("head")
        let sourceNode = self.childNodeWithName("source")
        switch shownPerspective {
        case let perspective where perspective == "top":
            sourceNodeHandler = TopSourceNodeHandler(node: sourceNode)
            
        case let perspective where perspective == "front":
            sourceNodeHandler = FrontSourceNodeHandler(node: sourceNode)
            
        case let perspective where perspective == "side":
            sourceNodeHandler = SideSourceNodeHandler(node: sourceNode)
            
        default:
            sourceNodeHandler = SourceNodeHandler(node: sourceNode)
        }
        
        headNode!.runAction(SKAction.setTexture(sourceNodeHandler?.getHeadTexture()))
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
            sourceNodeHandler?.changePosition(location)
        }
    }
    
    override func touchesEnded(touches: NSSet!, withEvent event: UIEvent!) {
        /* Called when a touch ends */
        touchesMoved(touches, withEvent: event)
        currentTouch = nil
    }
   
    override func update(currentTime: CFTimeInterval) {
        /* Called before each frame is rendered */
        if let handler = sourceNodeHandler {
            handler.updatePosition()
            
            if handler.hasChanged {
                
                handler.sourceNode.zRotation = handler.sourceNode.bearingTowards(headNode!)
                handler.hasChanged = false
            }
        }
    }
}
