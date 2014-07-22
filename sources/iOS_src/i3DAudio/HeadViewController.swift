//
//  HeadViewController.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 21/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit

class HeadViewController: UIViewController {
    override func viewWillAppear(animated: Bool) {
        OpenCVExternalCompatibility.test(self.view as UIImageView)
    }
}
