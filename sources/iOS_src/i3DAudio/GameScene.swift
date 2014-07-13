//
//  GameScene.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 13/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import SpriteKit

class GameScene: SKScene {
    override func didMoveToView(view: SKView) {
        /* Setup your scene here */
        let myLabel = SKLabelNode(fontNamed:"Chalkduster")
        myLabel.text = "Hello, World!";
        myLabel.fontSize = 65;
        myLabel.position = CGPoint(x:CGRectGetMidX(self.frame), y:CGRectGetMidY(self.frame));
        self.addChild(myLabel)
        
        let head = SKSpriteNode(imageNamed: "Head")
        head.xScale = 0.1
        head.yScale = 0.1
        head.position = CGPoint(x: self.size.width/2, y: self.size.height/2 )
        head.zRotation = CGFloat(M_PI)
        self.addChild(head)
        
        let speaker = SKSpriteNode(imageNamed: "Speaker")
        speaker.xScale = 0.1
        speaker.yScale = 0.1
        speaker.position = CGPoint(x: self.size.width/2, y: self.size.height*3/4 )
        speaker.zRotation = -CGFloat(M_PI) / 2
        self.addChild(speaker)
    }
    
    override func touchesBegan(touches: NSSet, withEvent event: UIEvent) {
        /* Called when a touch begins */
        
        for touch: AnyObject in touches {
            let location = touch.locationInNode(self)
            
            let sprite = SKSpriteNode(imageNamed:"Spaceship")
            
            sprite.xScale = 0.5
            sprite.yScale = 0.5
            sprite.position = location
            
            let action = SKAction.rotateByAngle(CGFloat(M_PI), duration:1)
            
            sprite.runAction(SKAction.repeatActionForever(action))
            
            self.addChild(sprite)
        }
    }
   
    override func update(currentTime: CFTimeInterval) {
        /* Called before each frame is rendered */
    }
}
