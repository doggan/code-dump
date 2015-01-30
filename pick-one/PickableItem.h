//
//  PickableItem.h
//  PickOne
//
//  Created by Shyam Guthikonda on 4/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
	PriorityDisabled = 0,
	PriorityLow,
	PriorityNormal,
	PriorityHigh,
} Priority;

@interface PickableItem : NSObject {
	NSString* title;
	Priority priority;
}

@property (nonatomic, retain) NSString* title;
@property (nonatomic) Priority priority;

- (id)initWithTitle:(NSString*)inTitle;
+ (NSString*)priorityToString:(Priority)inPriority;
+ (Priority)lookUpPriority:(NSInteger)inNumber;

@end
