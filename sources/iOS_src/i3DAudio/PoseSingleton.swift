//
//  SourceData.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 17/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit

protocol SimpleObserver {
    func update(object: AnyObject, property: String)
}

class PositionSingleton: NSObject {
    struct Static {
        static var token : dispatch_once_t = 0
        static var instance : PositionSingleton?
    }
    
    class var instance: PositionSingleton {
    dispatch_once(&Static.token) {  Static.instance = PositionSingleton() }
        return Static.instance!
    }
    
    let radToDeg: CGFloat = 180.0 / CGFloat(M_PI)
    let distanceScale: CGFloat = 200
    
    var delegate: SimpleObserver?
    
    var elevation: CGFloat = 0.0 {
    didSet {
        delegate?.update(self, property: "elevation")
    }
    }
    var azimuth: CGFloat = 0.0 {
    didSet {
        delegate?.update(self, property: "azimuth")
    }
    }
    var distance: CGFloat = 1.0 {
    didSet {
        delegate?.update(self, property: "distance")
    }
    }
    
    func setCartesianCoordinates(#x: CGFloat, y: CGFloat, z: CGFloat) {
        // Handle here the constraints
        
        let distancePixels = sqrt(x*x + y*y + z*z)
        distance = max(distancePixels / distanceScale, 0.3)
        elevation = max(asin(z / distancePixels) * radToDeg, -40)
        azimuth = atan2(y, x) * radToDeg
    }
    
    func setCartesianCoordinates(#x: CGFloat, y: CGFloat) {
        let actualZ = getCartesianCoordinates().z
        setCartesianCoordinates(x: x, y: y, z: actualZ)
    }
    
    func setCartesianCoordinates(#x: CGFloat, z: CGFloat) {
        let actualY = getCartesianCoordinates().y
        setCartesianCoordinates(x: x, y: actualY, z: z)
    }
    
    func setCartesianCoordinates(#y: CGFloat, z: CGFloat) {
        let actualX = getCartesianCoordinates().x
        setCartesianCoordinates(x: actualX, y: y, z: z)
    }
    
    func getCartesianCoordinates() -> (x: CGFloat, y:CGFloat, z:CGFloat) {
        let distancePixels = distance * distanceScale
        let elevationDeg = elevation / radToDeg
        let azimuthDeg = azimuth / radToDeg
        
        return (distancePixels * cos(azimuthDeg) * cos(elevationDeg), distancePixels * sin(azimuthDeg) * cos(elevationDeg), distancePixels * sin(elevationDeg))
    }
}

class OrientationSingleton: NSObject {
    struct Static {
        static var token : dispatch_once_t = 0
        static var instance : OrientationSingleton?
    }
    
    class var instance: OrientationSingleton {
    dispatch_once(&Static.token) {  Static.instance = OrientationSingleton() }
        return Static.instance!
    }
    
    let radToDeg: CGFloat = 180.0 / CGFloat(M_PI)
    
    var delegate: SimpleObserver?
    
    var yaw: CGFloat = 0.0 {
    didSet {
        delegate?.update(self, property: "yaw")
    }
    }
    var pitch: CGFloat = 0.0 {
    didSet {
        delegate?.update(self, property: "pitch")
    }
    }
    var roll: CGFloat = 0.0 {
    didSet {
        delegate?.update(self, property: "roll")
    }
    }
    
    func setYawRadians(newYaw: CGFloat) {
        yaw = newYaw * radToDeg
    }
    
    func setPitchRadians(newPitch: CGFloat) {
        pitch = newPitch * radToDeg
    }
    
    func setRollRadians(newRoll: CGFloat) {
        roll = newRoll * radToDeg
    }
    
    
    func getYawRadians() -> CGFloat {
        return yaw / radToDeg
    }
    
    func getPitchRadians() -> CGFloat {
        return pitch / radToDeg
    }
    
    func getRollRadians() -> CGFloat {
        return roll / radToDeg
    }
}