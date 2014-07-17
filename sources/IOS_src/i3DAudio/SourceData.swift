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
    
    var elevation = 0.0
    var azimuth = 0.0
    var distance = 0.0
    
    func setCartesianCoordinates(#x: Double, y: Double, z: Double) {
        azimuth = atan2(y, x)
        distance = sqrt(x*x + y*y + z*z)
        elevation = asin(z / distance)
    }
    
    func setCartesianCoordinates(#x: Double, y: Double) {
        let savedElevation = elevation
        setCartesianCoordinates(x: x, y: y, z: 0) // TODO Find out why it must be like this
        elevation = savedElevation
    }
    
    func setCartesianCoordinates(#x: Double, z: Double) {
        let actualY = getCartesianCoordinates().y
        setCartesianCoordinates(x: x, y: actualY, z: z)
    }
    
    func setCartesianCoordinates(#y: Double, z: Double) {
        let actualX = getCartesianCoordinates().x
        setCartesianCoordinates(x: actualX, y: y, z: z)
    }
    
    func getCartesianCoordinates() -> (x: Double, y:Double, z:Double) {
        return (distance * cos(azimuth) * cos(elevation), distance * sin(azimuth) * cos(elevation), distance * sin(elevation))
    }
}
