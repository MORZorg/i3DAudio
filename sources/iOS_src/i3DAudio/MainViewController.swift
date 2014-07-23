//
//  MainViewController.swift
//  i3DAudio
//
//  Created by Maurizio Zucchelli on 22/07/14.
//  Copyright (c) 2014 MORZorg. All rights reserved.
//

import UIKit
import MediaPlayer

class MainViewController: UIViewController, MPMediaPickerControllerDelegate {
    let audioController = AudioController()

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        audioController.openDefault()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func chooseSource(sender: AnyObject) {
        let picker = MPMediaPickerController(mediaTypes: MPMediaType.AnyAudio)
        picker.delegate = self
        picker.allowsPickingMultipleItems = false
        picker.prompt = NSLocalizedString("MainMediaPicker.title", tableName: nil, bundle: NSBundle.mainBundle(), value: "", comment: "")
        
        presentViewController(picker, animated: true, completion: nil)
    }
    
    @IBAction func resetSource(sender: AnyObject) {
        audioController.openDefault()

        PositionSingleton.instance.distance = 1
        PositionSingleton.instance.elevation = 0
        PositionSingleton.instance.azimuth = 0

        OrientationSingleton.instance.yaw = 0
        OrientationSingleton.instance.pitch = 0
        OrientationSingleton.instance.roll = 0
    }
    
    func mediaPicker(mediaPicker: MPMediaPickerController!, didPickMediaItems mediaItemCollection: MPMediaItemCollection!) {
        audioController.openFile(mediaItemCollection.representativeItem.assetURL)
        
        dismissViewControllerAnimated(true, completion: nil)
    }
    
    func mediaPickerDidCancel(mediaPicker: MPMediaPickerController!) {
        dismissViewControllerAnimated(true, completion: nil)
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue!, sender: AnyObject!) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
