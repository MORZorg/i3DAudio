//
//  GameScene.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 13/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import SpriteKit

extension CGPoint {
    func bearingTowards(point: CGPoint) -> CGFloat {
        return atan2(point.y - y, point.x - x)
    }
}

class SourceScene: SKScene {
    var shownPerspective: String?
    
    var headNodeHandler: HeadNodeHandler?
    var headTouch: AnyObject?
    
    var sourceNodeHandler: SourceNodeHandler?
    var sourceTouch: AnyObject?
    
    override func didMoveToView(view: SKView) {
        let headNode = self.childNodeWithName("head")!
        let sourceNode = self.childNodeWithName("source")!
        switch shownPerspective {
        case let perspective where perspective == "top":
            headNodeHandler = TopHeadNodeHandler(node: headNode)
            sourceNodeHandler = TopSourceNodeHandler(node: sourceNode)
            
        case let perspective where perspective == "front":
            headNodeHandler = FrontHeadNodeHandler(node: headNode)
            sourceNodeHandler = FrontSourceNodeHandler(node: sourceNode)
            
        case let perspective where perspective == "side":
            headNodeHandler = SideHeadNodeHandler(node: headNode)
            sourceNodeHandler = SideSourceNodeHandler(node: sourceNode)
            
        default:
            headNodeHandler = HeadNodeHandler(node: headNode)
            sourceNodeHandler = SourceNodeHandler(node: sourceNode)
        }
        
        headNode.runAction(SKAction.setTexture(headNodeHandler!.getTexture()!))
    }
    
    override func touchesBegan(touches: NSSet, withEvent event: UIEvent) {
        /* Called when a touch begins */
        for touch in touches {
            if sourceNodeHandler?.node == nodeAtPoint(touch.locationInNode(self)) {
                sourceTouch = touch
            }
            else if headNodeHandler?.node == nodeAtPoint(touch.locationInNode(self)) {
                headTouch = touch
            }
        }
        if headTouch == nil {
            sourceTouch = touches.anyObject()
        }
    }
    
    override func touchesMoved(touches: NSSet, withEvent event: UIEvent) {
        /* Called when a touch moves */
        if let location = sourceTouch?.locationInNode(self) {
            sourceNodeHandler?.changePosition(location)
        }
        if let location = headTouch?.locationInNode(self) {
            headNodeHandler?.changeRotation(headNodeHandler!.node.position.bearingTowards(location))
        }
    }
    
    override func touchesEnded(touches: NSSet, withEvent event: UIEvent) {
        /* Called when a touch ends */
        touchesMoved(touches, withEvent: event)
        headTouch = nil
        sourceTouch = nil
    }
   
    override func update(currentTime: CFTimeInterval) {
        /* Called before each frame is rendered */
        if let sourceHandler = sourceNodeHandler {
            if let headHandler = headNodeHandler {
                sourceHandler.updatePosition()
                headHandler.updateRotation()
                
                if sourceHandler.hasChanged {
                    sourceHandler.node.zRotation = sourceHandler.node.position.bearingTowards(headHandler.node.position)
                    sourceHandler.hasChanged = false
                }
            }
        }
    }
}
