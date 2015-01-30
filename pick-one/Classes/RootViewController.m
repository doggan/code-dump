//
//  RootViewController.m
//  PickOne
//
//  Created by Shyam Guthikonda on 4/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "RootViewController.h"
#import "PickableItemList.h"
#import "PickableItemViewController.h"
#import "EditPickableListViewController.h"

@implementation RootViewController

@synthesize editPickableListViewController;
@synthesize pickableItemViewController;
@synthesize pickableItemLists;

#pragma mark -
#pragma mark View lifecycle

- (void)viewDidLoad {
    [super viewDidLoad];
	
	// Title.
	self.title = @"PickOne";
	
	// Edit button.
    self.navigationItem.rightBarButtonItem = self.editButtonItem;
	
	// New button.
	self.navigationItem.leftBarButtonItem = [[[UIBarButtonItem alloc]
											  initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
											  target:self
											  action:@selector(addTapped:)]
											 autorelease];
	
	// Selecting a list during editing will take us to the list edit screen.
	self.tableView.allowsSelectionDuringEditing = YES;
}

- (void)viewWillAppear:(BOOL)animated {
	[self.tableView reloadData];
    [super viewWillAppear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Disable auto-rotate.
	return NO;
}

#pragma mark -
#pragma mark Table view data source

// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.pickableItemLists.count;
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc]
				 initWithStyle:UITableViewCellStyleDefault
				 reuseIdentifier:CellIdentifier]
				autorelease];
    }
    
	// Configure the cell.
	PickableItemList* list = [self.pickableItemLists objectAtIndex:indexPath.row];
	cell.textLabel.text = list.listName;
	cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;

    return cell;
}

// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source.
        [self.pickableItemLists removeObjectAtIndex:indexPath.row];
		[tableView 
		 deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]
		 withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
    }   
}

- (void)changeToEditListView:(PickableItemList*)inList {
	if (self.editPickableListViewController == nil) {
		self.editPickableListViewController = [[[EditPickableListViewController alloc]
												initWithNibName:@"EditPickableListViewController"
												bundle:[NSBundle mainBundle]]
											   autorelease];
	}
	
	self.editPickableListViewController.pickableItemList = inList;
	[self.navigationController pushViewController:self.editPickableListViewController animated:YES];
}

- (void)addTapped:(id)sender {
	PickableItemList* list = [[[PickableItemList alloc]
						   initWithName:@"New List"]
						  autorelease];
	[self.pickableItemLists addObject:list];
	
	NSIndexPath* indexPath = [NSIndexPath indexPathForRow:self.pickableItemLists.count - 1 inSection:0];
	NSArray* indexPaths = [NSArray arrayWithObject:indexPath];
	[self.tableView insertRowsAtIndexPaths:indexPaths withRowAnimation:YES];
	
	// Allow editing of the newly created list.
	[self changeToEditListView:list];
}

// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
	PickableItemList* listToMove = [[self.pickableItemLists objectAtIndex:fromIndexPath.row] retain];
	[self.pickableItemLists removeObjectAtIndex:fromIndexPath.row];
	[self.pickableItemLists insertObject:listToMove atIndex:toIndexPath.row];
	[listToMove release];
}

// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
	// Ignore if there is only a single item existing.
	if (self.pickableItemLists.count <= 1) {
		return NO;
	}
	
    // Allow re-orderable rows.
    return YES;
}

#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	if (self.editing) {
		// Edit list.
		PickableItemList* list = [pickableItemLists objectAtIndex:indexPath.row];
		[self changeToEditListView:list];
		self.editing = NO;
	}
	else {
		// Go to "PickOne" view.
		if (self.pickableItemViewController == nil) {
			self.pickableItemViewController = [[[PickableItemViewController alloc]
												initWithNibName:@"PickableItemViewController"
												bundle:[NSBundle mainBundle]]
											   autorelease];
		}
		
		// Set the list to be viewed.
		PickableItemList* list = [pickableItemLists objectAtIndex:indexPath.row];
		self.pickableItemViewController.pickableItemList = list;
		
		// Change views.
		[self.navigationController pushViewController:self.pickableItemViewController animated:YES];
	}
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
	self.editPickableListViewController = nil;
	self.pickableItemViewController = nil;
}

- (void)viewDidUnload {
	[super viewDidUnload];
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}

- (void)dealloc {
	self.editPickableListViewController = nil;
	self.pickableItemViewController = nil;
	self.pickableItemLists = nil;
    [super dealloc];
}

@end

