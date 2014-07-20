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

class SourceData: NSObject {
    struct Static {
        static var token : dispatch_once_t = 0
        static var instance : SourceData?
    }
    
    class var instance: SourceData {
        dispatch_once(&Static.token) {  Static.instance = SourceData() }
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
        distance = 1  // distancePixels / distanceScale
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
