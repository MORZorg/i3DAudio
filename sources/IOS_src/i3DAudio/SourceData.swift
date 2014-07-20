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

class SourceData {
    struct Static {
        static var token : dispatch_once_t = 0
        static var instance : SourceData?
    }
    
    class var instance: SourceData {
        dispatch_once(&Static.token) {  Static.instance = SourceData() }
        return Static.instance!
    }
    
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
        azimuth = atan2(y, x)
        distance = sqrt(x*x + y*y + z*z)
        elevation = asin(z / distance)
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
        return (distance * cos(azimuth) * cos(elevation), distance * sin(azimuth) * cos(elevation), distance * sin(elevation))
    }
}
