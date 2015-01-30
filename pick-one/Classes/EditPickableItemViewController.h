//
//  EditPickableItemViewController.h
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PickableItem;

@interface EditPickableItemViewController : UIViewController <	UITextFieldDelegate,
																UIPickerViewDelegate,
																UIPickerViewDataSource,
																UINavigationControllerDelegate > {
	PickableItem* pickableItem;
	UITextField* titleField;
																	
	IBOutlet UIPickerView* priorityPicker;
	NSMutableArray* priorityArray;
}

@property (nonatomic, retain) PickableItem* pickableItem;
@property (nonatomic, retain) IBOutlet UITextField* titleField;

- (IBAction)titleFieldValueChanged:(id)sender;

@end
