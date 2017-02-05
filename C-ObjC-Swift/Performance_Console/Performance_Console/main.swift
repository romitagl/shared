//
//  main.swift
//  Performance_Console
//
//  Created by Gian Luigi Romita on 11/06/14.
//  Copyright (c) 2014 Gian Luigi Romita. All rights reserved.
//

import Foundation

let num_elements  : NSInteger = 1000000

Performance_ObjCtoCPlusPlus.sortArrayCPlusPlus(num_elements)

Performance_ObjectiveC.sortArrayObjC(num_elements)

//println("Hello, World!")


var int_array = [Int](repeating: 0, count: num_elements)
for i in 0...(num_elements-1) {
    int_array[i] = Int(arc4random())
}

    // Put the code you want to measure the time of here.
let start : Date = Date()

let sorted_array = int_array.sort(by: <)

let end : Date = Date()

let executionTime : TimeInterval = end.timeIntervalSince(start)

print("swift executionTime: \(executionTime) seconds for \(num_elements) elements. \n", terminator: "");
