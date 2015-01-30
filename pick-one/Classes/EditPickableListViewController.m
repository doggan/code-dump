//
//  EditPickableListViewController.m
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "EditPickableListViewController.h"
#import "PickableItemList.h"

@implementation EditPickableListViewController

@synthesize pickableItemList;
@synthesize listNameField;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)viewWillAppear:(BOOL)animated {
	self.listNameField.text = self.pickableItemList.listName;
	[self.listNameField becomeFirstResponder];
	[super viewWillAppear:animated];
}

- (IBAction)listNameFieldValueChanged:(id)sender {
	self.pickableItemList.listName = self.listNameField.text;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
	
    // Release any retained subviews of the main view.
    self.listNameField = nil;
}

- (void)dealloc {
	self.listNameField = nil;
    [super dealloc];
}

@end
