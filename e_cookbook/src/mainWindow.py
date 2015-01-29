################################################################################
# E-Cookbook - electronic recipe organizer
# Copyright (C) 2006 Shyam "Doggan" Guthikonda.
#
# Email: shyamguth@gmail.com
# URL: http://shy.am
#
# Project Page: http://sourceforge.net/projects/ecookbook
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free 
# Software Foundation; either version 2 of the License, or (at your option) 
# any later version.
#
# This software is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this library; if not, write to the Free Software Foundation, Inc., 
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
################################################################################

import os

import wx
from pysqlite2 import dbapi2 as sqlite

from addModDialog import *
from searchPanel import *
from viewDialog import *
from aboutDialog import *

gAppTitle = "E-Cookbook"
gAppVersion = "v1.0.0b"
gAppTitleAndVersion = gAppTitle + " " + gAppVersion
gAppSize = (950, 600)

# File.
ID_NEW_COOKBOOK = wx.NewId()
ID_OPEN_COOKBOOK = wx.NewId()
ID_SAVE = wx.NewId()
ID_EXIT = wx.NewId()

# Help.
ID_HELP = wx.NewId()
ID_ABOUT = wx.NewId()

# Buttons
ID_BUTTON_ADD_RECIPE = wx.NewId()
ID_BUTTON_DELETE_RECIPE = wx.NewId()
ID_BUTTON_MODIFY_RECIPE = wx.NewId()
ID_BUTTON_VIEW_RECIPE = wx.NewId()

# Main table name.
gNumDBCols = 12
gTableName = "recipeTable_" + str(gNumDBCols)

# Master list of columns in the database.
# Note: It's probably not safe to adda column to the middle of this. Add to the end if you must.
gDBColsMaster = ["recipeID", "name", "category", "cuisine", "rating", "prepTime", "cookTime", "source",
                 "ingTableName",
                 "instructions", "extraNotes", "servings"
                ]

# List of database columns that we want to be displayed in the main listView
gTableCols = [gDBColsMaster[0], gDBColsMaster[1], gDBColsMaster[2], gDBColsMaster[3],
              gDBColsMaster[4], gDBColsMaster[5], gDBColsMaster[6], gDBColsMaster[7]]

class MainWindow(wx.Frame):
    def __init__(self, parent, id, title, appSize):
        wx.Frame.__init__(self, parent, wx.ID_ANY, title, pos = wx.DefaultPosition,
                                        size = appSize,
                                        style = wx.DEFAULT_FRAME_STYLE)
                                   
        self.__origTitle = self.GetTitle()
           
        self.CreateStatusBar()
        self.__buildMenu()
        self.__buildLayout()
        
        self.__databaseLoaded = False
        self.__toggleButtons(self.__databaseLoaded)
        self.__isModified = False
        
        self.__newFile = False
        self.__everSaved = False
        self.__curLoadedPath = ""
        
        self.Show(True)
        
    def getIsDBLoaded(self):
        return self.__databaseLoaded
    
    def getConnection(self):
        if self.getIsDBLoaded():
            return self.__con
        assert("No DB connection!")
        
    def __buildMenu(self):
        # File.
        fileMenu = wx.Menu()
        fileMenu.Append(ID_NEW_COOKBOOK, "&New Cookbook", " Create a new recipe database")
        fileMenu.Append(ID_OPEN_COOKBOOK, "&Open Cookbook", "Open an existing database")
        fileMenu.AppendSeparator()
        fileMenu.Append(ID_SAVE, "&Save Cookbook\tCtrl+S", " Save the cookbook out to a file.")
        fileMenu.AppendSeparator()
        fileMenu.Append(ID_EXIT, "&Quit", " Terminate the program")
        
        wx.EVT_MENU(self, ID_NEW_COOKBOOK, self.__onNewCookbook)
        wx.EVT_MENU(self, ID_OPEN_COOKBOOK, self.__onOpenCookbook)
        wx.EVT_MENU(self, ID_SAVE, self.__onSave)
        wx.EVT_MENU(self, ID_EXIT, self.__onExit)
        
        wx.EVT_CLOSE(self, self.__onExit)   # Handle clicking the 'X' on the window
        
        # Help.
        helpMenu = wx.Menu()
        helpMenu.Append(ID_HELP, "&Help Me!", " Help using this application")
        helpMenu.Append(ID_ABOUT, "&About", " Information about this application")
        
        wx.EVT_MENU(self, ID_HELP, self.__onHelp)
        wx.EVT_MENU(self, ID_ABOUT, self.__onAbout)

        # Menu bar.
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "&File")
        menuBar.Append(helpMenu, "&Help")
        self.SetMenuBar(menuBar)

    def __toggleButtons(self, state):
        self.__buttonAddRec.Enable(state)
        self.__buttonDelRec.Enable(state)
        self.__buttonModRec.Enable(state)
        self.__buttonViewRec.Enable(state)
       
    def __onNewCookbook(self, e):
        dlg = wx.FileDialog(self, "New Cookbook", "", "", "E-Cookbook File (*.cb)|*.cb", wx.SAVE | wx.OVERWRITE_PROMPT)
        
        # Prompt for saving of any currently loaded database.
        if self.__isModified:
            retCode = self.__promptForSave("Would you like to save your changes to the currently open cookbook before opening a new one?")
            if retCode == wx.YES:
                self.__onSave(e)
            elif retCode == wx.CANCEL:
                return
        
        if dlg.ShowModal() == wx.ID_OK:
            fileName = dlg.GetFilename()
            dirName = dlg.GetDirectory()
            
            # Verify extension (we have to do this manually as OS X isn't adding the extension).
            if fileName[-3:] != ".cb":
                fileName = fileName[:] + ".cb"
            
            fullPath = dirName + '/' + fileName
            prevPath = self.__curLoadedPath
            self.__curLoadedPath = fullPath
            
            # Unload any current database
            self.__unloadCurrent()
            
            # Delete any file that was created (New) and never saved from the disk.
            if self.__newFile == True and self.__everSaved == False:
                utils.deleteFile(prevPath)
            
            # Set the title bar to be the filename w/o extension.
            self.SetTitle(self.__origTitle + " : " + fileName[:-3])
            
            self.__newFile = True
            
            # If we are overwriting a file, we need to delete the old file first so that
            # we can create the new file.
            utils.deleteFile(self.__curLoadedPath)
            
            self.__con = sqlite.connect(fullPath, isolation_level = None)
            
            cur = self.__con.cursor()
            cur.execute("BEGIN TRANSACTION mainTrans")
        
            # Comma between each column name
            colsStr = ','.join(gDBColsMaster[1:])
        
            # Create the actual table
            CREATE_DB = "create table " + gTableName + " ( " + gDBColsMaster[0] + " INTEGER PRIMARY KEY, " + colsStr + ");"
            cur.execute(CREATE_DB)
        
            self.__isModified = True
            self.__databaseLoaded = True
            self.__toggleButtons(self.__databaseLoaded)
        
    def __onOpenCookbook(self, e):
        dirName = ''
        dlg = wx.FileDialog(self, "Choose a cookbook", dirName, "", "E-Cookbook File (*.cb)|*.cb", wx.OPEN)
        
        # Prompt for saving of any currently loaded database.
        if self.__isModified:
            retCode = self.__promptForSave("Would you like to save your changes to the currently open cookbook before opening a new one?")
            if retCode == wx.YES:
                self.__onSave(e)
            elif retCode == wx.CANCEL:
                return
        
        if dlg.ShowModal() == wx.ID_OK:
            fileName = dlg.GetFilename()
            dirName = dlg.GetDirectory()
            
            fullPath = dirName + '/' + fileName
            
            # That database is alrdy open.
            if fullPath == self.__curLoadedPath:
                msg = "The selected cookbook is already open!"
                caption = "Invalid Cookbook selected."
                aldryDlg = msgBoxError(self, msg, caption)
                aldryDlg.ShowModal()
                aldryDlg.Destroy()
                dlg.Destroy()
                return
            
            if utils.isValidLocation(fullPath):
                con = sqlite.connect(fullPath, isolation_level = None)
            
                # If this is a valid file, open it.
                if self.__verifyCon(con):
                    self.__unloadCurrent()
                
                    self.__con = con
                    self.__databaseLoaded = True
                    cur = self.__con.cursor()
                    cur.execute("BEGIN TRANSACTION mainTrans")
                
                    # Set the title bar to be the filename w/o extension.
                    self.SetTitle(self.__origTitle + " : " + fileName[:-3])
                
                    prevPath = self.__curLoadedPath
                    self.__curLoadedPath = fullPath
                
                    # Delete any file that was created (New) and never saved from the disk.
                    if self.__newFile == True and self.__everSaved == False:
                        utils.deleteFile(prevPath)
                
                    self.__newFile = False
                    self.__everSaved = False
                    self.__isModified = False
                # Else it's not a valid file. Just keep the current file (if any) open
                else:
                    msg = "Cookbooks must be created from within this application in order to be valid."
                    caption = "Invalid Cookbook selected."
                    erDlg = msgBoxError(self, msg, caption)
                    erDlg.ShowModal()
                    erDlg.Destroy()
            else:
                msg = "Cannot open the file, as it does not yet exist!"
                caption = "Invalid Filename entered."
                invDlg = msgBoxError(self, msg, caption)
                invDlg.ShowModal()
                invDlg.Destroy()
            
        dlg.Destroy()
        
        self.__toggleButtons(self.__databaseLoaded)
        
        if self.__databaseLoaded:
            self.__searchPanel.update(self.__con)
        
    def __onSave(self, e):
        if self.__isModified:
            self.__isModified = False
            
            # Close current transaction and begin a new one.
            cur = self.__con.cursor()
            cur.execute("END TRANSACTION mainTrans")
            
            cur.execute("BEGIN TRANSACTION mainTrans")
            
            self.__con.commit()
            
        self.__everSaved = True
        
    def __onExit(self, e):
        if self.__isModified:
            retCode = self.__promptForSave("Would you like to save your changes before exiting?")
            if retCode == wx.YES:
                self.__onSave(e)
            elif retCode == wx.CANCEL:
                return
            
        self.Destroy()
        
        try:
            self.__con.close()
        except AttributeError:
            pass
            
        # Delete any file that was created (New) and never saved from the disk.
        if self.__newFile == True and self.__everSaved == False:
            utils.deleteFile(self.__curLoadedPath)
        
    def __onAbout(self, e):
        dlg = AboutDialog(self, wx.ID_ANY, "About " + gAppTitleAndVersion)
        dlg.ShowModal()
        dlg.Destroy()
        
    def __onHelp(self, e):
        # TODO: couldn't figure out a cross-platform way to open the default browser, so left at this.
        helpDlg = wx.MessageDialog(self, "For user-manual, see: docs/manual.html", "User Manual", wx.OK | wx.ICON_INFORMATION)
        helpDlg.ShowModal()
        helpDlg.Destroy()

    def __onClickAddRecipe(self, e):
        dlg = AddModDialog(self, wx.ID_ANY, "Add New Recipe", self.__con, "add")
                
        if dlg.ShowModal() == wx.ID_OK:
            self.__isModified = True
            self.__searchPanel.update(self.__con)
        
        dlg.Destroy()
        
    def __onClickDeleteRecipe(self, e):
        # Verify items are selected
        if self.__searchPanel.getNumSelected() == 0:
            errDlg = wx.MessageDialog(self, "No items selected to delete.", "Invalid Selection", wx.OK | wx.ICON_ERROR)
            errDlg.ShowModal()
            errDlg.Destroy()
            return
        
        retCode = wx.MessageBox("Are you sure you want to delete the selected recipes?", "Delete Confirmation", wx.YES_NO | wx.ICON_QUESTION, self)
        if retCode != wx.YES:
            return
        
        self.__isModified = True
        self.__searchPanel.deleteSelected(self.__con)
    
    def __onClickModifyRecipe(self, e):
        # Verify 1 item is selected
        if self.__searchPanel.getNumSelected() > 1 or self.__searchPanel.getNumSelected() == 0:
            errDlg = wx.MessageDialog(self, "You must select exactly 1 recipe to modify.", "Invalid Selection", wx.OK | wx.ICON_ERROR)
            errDlg.ShowModal()
            errDlg.Destroy()
            return
            
        dlg = AddModDialog(self, wx.ID_ANY, "Modify Recipe", self.__con, "mod", self.__searchPanel.getUniqueID()[0])
        
        if dlg.ShowModal() == wx.ID_OK:
            self.__isModified = True
            self.__searchPanel.update(self.__con)    # TODO: could make more efficient by just updating changed entry...
         #   self.__searchPanel.update(self.__con, selectedIndex)
        
        dlg.Destroy()
    
    def __onClickViewRecipe(self, e):
        # Verify 1 item is selected
        if self.__searchPanel.getNumSelected() > 1 or self.__searchPanel.getNumSelected() == 0:
            errDlg = wx.MessageDialog(self, "You must select exactly 1 recipe to view.", "Invalid Selection", wx.OK | wx.ICON_ERROR)
            errDlg.ShowModal()
            errDlg.Destroy()
            return
        
        self.__searchPanel.getUniqueID()
        dlg = ViewDialog(self, wx.ID_ANY, "View Recipe", self.__con, self.__searchPanel.getUniqueID()[0])
        
        if dlg.ShowModal() == wx.ID_OK:
            pass
        
        dlg.Destroy()
        
    def __buildLayout(self):
        self.__mainPanel = wx.Panel(self, wx.ID_ANY)
        
        # Buttons
        self.__buttonAddRec = wx.Button(self.__mainPanel, ID_BUTTON_ADD_RECIPE, "Add Recipe")
        wx.EVT_BUTTON(self.__mainPanel, ID_BUTTON_ADD_RECIPE, self.__onClickAddRecipe)
        self.__buttonDelRec = wx.Button(self.__mainPanel, ID_BUTTON_DELETE_RECIPE, "Delete Recipe")
        wx.EVT_BUTTON(self.__mainPanel, ID_BUTTON_DELETE_RECIPE, self.__onClickDeleteRecipe)
        self.__buttonModRec = wx.Button(self.__mainPanel, ID_BUTTON_MODIFY_RECIPE, "Modify Recipe")
        wx.EVT_BUTTON(self.__mainPanel, ID_BUTTON_MODIFY_RECIPE, self.__onClickModifyRecipe)
        self.__buttonViewRec = wx.Button(self.__mainPanel, ID_BUTTON_VIEW_RECIPE, "View Recipe")
        wx.EVT_BUTTON(self.__mainPanel, ID_BUTTON_VIEW_RECIPE, self.__onClickViewRecipe)
        
        # Search box.
        self.__searchPanel = SearchPanel(self.__mainPanel, self)
        
        # Sizers.
        topsizer = wx.BoxSizer(wx.HORIZONTAL)
        buttonSizer = wx.BoxSizer(wx.VERTICAL)
        buttonSizer.Add(self.__buttonAddRec, 0, wx.ALIGN_CENTER | wx.EXPAND | wx.ALL, 5)
        buttonSizer.Add(self.__buttonDelRec, 0, wx.ALIGN_CENTER | wx.EXPAND | wx.ALL, 5)
        buttonSizer.Add(self.__buttonModRec, 0, wx.ALIGN_CENTER | wx.EXPAND | wx.ALL, 5)
        buttonSizer.Add(self.__buttonViewRec, 0, wx.ALIGN_CENTER | wx.EXPAND | wx.ALL, 5)
       
        topsizer.Add(buttonSizer, 0, wx.ALIGN_CENTER_VERTICAL | wx.ALIGN_CENTER | wx.WEST, 10)
        topsizer.Add(self.__searchPanel, 1, wx.EXPAND | wx.ALL | wx.ALIGN_CENTER | wx.ALIGN_CENTER_VERTICAL, 20)
        
        self.__mainPanel.SetSizerAndFit(topsizer)
        
    # Resets the UI to 'default' status. Clears any existing entries, etc.
    def __unloadCurrent(self):
        try:
            self.__con.close()
        except AttributeError:
            pass
        
        # Clear search panel
        self.__searchPanel.clearAll()
        
    # Given a SQLite connection, determine if the database is valid.
    # Returns true for valid connection, false for invalid.
    def __verifyCon(self, connection):
        # TODO: figure out some better system for this.
        try:
            # If the table exists, it's a valid database.
            cur = connection.cursor()
            cur.execute("select * from " + gTableName)
            return True
        except sqlite.OperationalError:
            return False
          
    def __promptForSave(self, msg):
        return wx.MessageBox(msg, "Unsaved Changes", wx.YES_NO | wx.CANCEL | wx.ICON_QUESTION, self)
            
def msgBoxInfo(parent, msg, caption):
    return wx.MessageDialog(parent, msg, caption, wx.OK | wx.ICON_INFORMATION)
    
def msgBoxError(parent, msg, caption):
    return wx.MessageDialog(parent, msg, caption, wx.OK | wx.ICON_ERROR)
   
if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = MainWindow(None, -1, gAppTitleAndVersion, gAppSize)
    app.MainLoop()