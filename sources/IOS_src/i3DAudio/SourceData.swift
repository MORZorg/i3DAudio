//
//  SourceData.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 17/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit

class SourceData {
    struct Static {
        static var token : dispatch_once_t = 0
        static var instance : SourceData?
    }
    
    class var instance: SourceData {
        dispatch_once(&Static.token) {  Static.instance = SourceData() }
        return Static.instance!
    }
    
    var elevation: Float = 0.0
    var azimuth: Float = 0.0
    var distance: Float = 1.0
    
    func setCartesianCoordinates(#x: Float, y: Float, z: Float) {
        azimuth = atan2(y, x)
        distance = sqrt(x*x + y*y + z*z)
        elevation = asin(z / distance)
    }
    
    func setCartesianCoordinates(#x: Float, y: Float) {
        let actualZ = getCartesianCoordinates().z
        setCartesianCoordinates(x: x, y: y, z: actualZ)
    }
    
    func setCartesianCoordinates(#x: Float, z: Float) {
        let actualY = getCartesianCoordinates().y
        setCartesianCoordinates(x: x, y: actualY, z: z)
    }
    
    func setCartesianCoordinates(#y: Float, z: Float) {
        let actualX = getCartesianCoordinates().x
        setCartesianCoordinates(x: actualX, y: y, z: z)
    }
    
    func getCartesianCoordinates() -> (x: Float, y:Float, z:Float) {
        return (distance * cos(azimuth) * cos(elevation), distance * sin(azimuth) * cos(elevation), distance * sin(elevation))
    }
}
