//
//  PickableItemViewController.m
//  PickOne
//
//  Created by Shyam Guthikonda on 4/23/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "PickableItemViewController.h"
#import "EditPickableItemViewController.h"
#import "PickableItemList.h"
#import "PickableItem.h"

@implementation PickableItemViewController

@synthesize editPickableItemViewController;
@synthesize pickableItemList;

@synthesize pickOneButton;

#pragma mark -
#pragma mark Setup

- (void)viewDidLoad {
    [super viewDidLoad];
	
	// Title.
	self.title = @"PickOne2";
	
	// Edit button.
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
	// New button.
	self.navigationItem.leftBarButtonItem = [[[UIBarButtonItem alloc]
											  initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
											  target:self
											  action:@selector(addTapped:)]
											 autorelease];
}

- (void)viewWillAppear:(BOOL)animated {
	[self.tableView reloadData];
    [super viewWillAppear:animated];
}

#pragma mark -
#pragma mark Appearance

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Disable auto-rotate.
	return NO;
}

// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

#pragma mark -
#pragma mark Table view data source

// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self.pickableItemList itemCount];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc]
				 initWithStyle:UITableViewCellStyleSubtitle
				 reuseIdentifier:CellIdentifier]
				autorelease];
    }
    
	// Configure the cell.
	PickableItem* item = [self.pickableItemList objectAtIndex:indexPath.row];
	cell.textLabel.text = item.title;
	cell.detailTextLabel.text = [PickableItem priorityToString:[item priority]];
	cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
	
    return cell;
}

// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source.
        [self.pickableItemList removeObjectAtIndex:indexPath.row];
		[tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
    }   
}

- (void)addTapped:(id)sender {
	PickableItem* item = [[[PickableItem alloc]
						   initWithTitle:@"New Item"]
						  autorelease];
	[self.pickableItemList addObject:item];
	
	NSIndexPath* indexPath = [NSIndexPath
							  indexPathForRow:[self.pickableItemList itemCount] - 1
							  inSection:0];
	NSArray* indexPaths = [NSArray arrayWithObject:indexPath];
	[self.tableView insertRowsAtIndexPaths:indexPaths withRowAnimation:YES];
	[self.tableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionMiddle];
	[self tableView:self.tableView didSelectRowAtIndexPath:indexPath];
}

// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
	PickableItem* itemToMove = [[self.pickableItemList objectAtIndex:fromIndexPath.row] retain];
	[self.pickableItemList removeObjectAtIndex:fromIndexPath.row];
	[self.pickableItemList insertObject:itemToMove atIndex:toIndexPath.row];
	[itemToMove release];
}

// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
	// Ignore if there is only a single item existing.
	if ([self.pickableItemList itemCount] <= 1) {
		return NO;
	}
	
    // Allow re-orderable rows.
    return YES;
}

- (IBAction)pickOneButtonTouched {
	// No items in list - return w/o doing anything.
	if ([self.pickableItemList itemCount] == 0) {
		return;
	}
	
	NSMutableArray* selectionList = [NSMutableArray arrayWithCapacity:[self.pickableItemList itemCount] * 2];

	// Create array filled with indices of items.
	// Use item priority as weight.
	for (int i = 0; i < [self.pickableItemList itemCount]; ++i) {
		PickableItem* item = [self.pickableItemList objectAtIndex:i];
		Priority itemPriority = [item priority];
		NSNumber* index = [NSNumber numberWithInt:i];
		
		switch (itemPriority) {
			case PriorityDisabled:
				break;
			case PriorityLow:
				[selectionList addObject:index];
				break;
			case PriorityNormal:
				[selectionList addObject:index];
				[selectionList addObject:index];
				break;
			case PriorityHigh:
				[selectionList addObject:index];
				[selectionList addObject:index];
				[selectionList addObject:index];
				break;
			default:
				break;
		}
	}
	
	// Selection.
	int rand = arc4random() % selectionList.count;
	int selectedIndex = [[selectionList objectAtIndex:rand] intValue];
	
	NSIndexPath* indexPath = [NSIndexPath indexPathForRow:selectedIndex inSection:0];	
	[self.tableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionMiddle];
//	[self tableView:self.tableView didSelectRowAtIndexPath:indexPath];
//	self.pickableItemList
	
}

#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (self.editPickableItemViewController == nil) {
		self.editPickableItemViewController = [[[EditPickableItemViewController alloc]
												initWithNibName:@"EditPickableItemViewController"
												bundle:[NSBundle mainBundle]]
											   autorelease];
	}
	
	PickableItem* item = [self.pickableItemList objectAtIndex:indexPath.row];
	self.editPickableItemViewController.pickableItem = item;
	[self.navigationController pushViewController:self.editPickableItemViewController animated:YES];
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
	self.editPickableItemViewController = nil;
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc {
	self.editPickableItemViewController = nil;
	self.pickableItemList = nil;
    [super dealloc];
}


@end
