//
//  RootViewController.h
//  PickOne
//
//  Created by Shyam Guthikonda on 4/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EditPickableListViewController;
@class PickableItemViewController;

@interface RootViewController : UITableViewController {
	EditPickableListViewController* editPickableListViewController;	// Allows for editing of list name.
	PickableItemViewController* pickableItemViewController; // Takes us to the main pickable view.
	NSMutableArray* pickableItemLists; // All the currently active lists.
}

@property (nonatomic, retain) EditPickableListViewController* editPickableListViewController;
@property (nonatomic, retain) PickableItemViewController* pickableItemViewController;
@property (nonatomic, retain) NSMutableArray* pickableItemLists;

@end
