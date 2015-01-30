//
//  PickableItemViewController.h
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EditPickableItemViewController;
@class PickableItemList;

@interface PickableItemViewController : UITableViewController {
	EditPickableItemViewController* editPickableItemViewController;
	PickableItemList* pickableItemList;
	
	UIButton* pickOneButton;
}

@property (nonatomic, retain) EditPickableItemViewController* editPickableItemViewController;
@property (nonatomic, retain) PickableItemList* pickableItemList;

@property (nonatomic, retain) IBOutlet UIButton* pickOneButton;

- (IBAction)pickOneButtonTouched;

@end
