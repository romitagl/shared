//
//  File.m
//  Performance_Console
//
//  Created by Gian Luigi Romita on 11/06/14.
//  Copyright (c) 2014 Gian Luigi Romita. All rights reserved.
//
#import "ObjC.h"

@implementation Performance_ObjectiveC

+ (void) sortArrayObjC: (NSInteger) array_size
{
    NSMutableArray* array = [NSMutableArray array];
    for(int i = 0; i < array_size; i++)
    {
        [array addObject: [NSNumber numberWithInt: arc4random()%array_size]];
    }
    
    NSDate *methodStart = [NSDate date];
    
    // ... Do whatever you need to do ...
    NSSortDescriptor* lowToHigh = [[NSSortDescriptor alloc ] initWithKey:@"self" ascending: YES];
    [array sortUsingDescriptors:[NSArray arrayWithObject:lowToHigh]];
    
    NSDate *methodFinish = [NSDate date];
    NSTimeInterval executionTime = [methodFinish timeIntervalSinceDate:methodStart];
    NSLog(@"ObjC executionTime: %f seconds for %ld elements. \n", executionTime, (long)array_size);
}

@end






