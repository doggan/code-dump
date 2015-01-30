//
//  EditPickableItemViewController.m
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "EditPickableItemViewController.h"
#import "PickableItem.h"

@implementation EditPickableItemViewController

@synthesize pickableItem;
@synthesize titleField;

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

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	priorityArray = [[NSMutableArray alloc] init];
	[priorityArray addObject:@"Disabled"];
	[priorityArray addObject:@"Low Priority"];
	[priorityArray addObject:@"Normal Priority"];
	[priorityArray addObject:@"High Priority"];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)viewWillAppear:(BOOL)animated {
	self.titleField.text = self.pickableItem.title;
//	[self.titleField becomeFirstResponder];
	
	// Initialize value of picker to currently set priority.
	[priorityPicker selectRow:self.pickableItem.priority inComponent:0 animated:NO];
	
	[super viewWillAppear:animated];
}

- (IBAction)titleFieldValueChanged:(id)sender {
	self.pickableItem.title = self.titleField.text;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
	[textField resignFirstResponder];
	return YES;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)thePickerView {	
	return 1;
}

- (NSInteger)pickerView:(UIPickerView *)thePickerView numberOfRowsInComponent:(NSInteger)component {	
	return [priorityArray count];
}

- (NSString *)pickerView:(UIPickerView *)thePickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
	return [priorityArray objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)thePickerView didSelectRow:(NSInteger)inRow inComponent:(NSInteger)inComponent {
	// Set the new priority.
	self.pickableItem.priority = [PickableItem lookUpPriority:inRow];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
	
    // Release any retained subviews of the main view.
	self.titleField = nil;
}

- (void)dealloc {
	self.pickableItem = nil;
	self.titleField = nil;
	priorityPicker = nil;
	[priorityArray release];
	priorityArray = nil;
    [super dealloc];
}

@end
