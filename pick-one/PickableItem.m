//
//  PickableItem.m
//  PickOne
//
//  Created by Shyam Guthikonda on 4/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "PickableItem.h"

@implementation PickableItem

@synthesize title;
@synthesize priority;

- (id)initWithTitle:(NSString *)inTitle {
	if (self = [super init])
	{
		self.title = inTitle;
		priority = PriorityNormal;
	}
	
	return self;
}

+ (NSString*)priorityToString:(Priority)inPriority {
	switch (inPriority) {
		case PriorityDisabled:
			return @"Disabled";
		case PriorityLow:
			return @"Low Priority";
		case PriorityNormal:
			return @"Normal Priority";
		case PriorityHigh:
			return @"High Priority";
	}
	
	return @"Disabled";
}

+ (Priority)lookUpPriority:(NSInteger)inNumber {
	if (inNumber >= 0 && inNumber <= PriorityHigh) {
		return (Priority)inNumber;
	}
	
	NSLog(@"Invalid priority: %i", inNumber);
	return PriorityNormal;
}

- (void) dealloc {
	self.title = nil;
	[super dealloc];
}

@end
