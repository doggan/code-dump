//
//  EditPickableListViewController.h
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PickableItemList;

@interface EditPickableListViewController : UIViewController <UITextFieldDelegate, UINavigationControllerDelegate> {
	PickableItemList* pickableItemList;
	UITextField* listNameField;
}

@property (nonatomic, retain) PickableItemList* pickableItemList;
@property (nonatomic, retain) IBOutlet UITextField* listNameField;

- (IBAction)listNameFieldValueChanged:(id)sender;

@end
