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

import sys
import wx
import string

from fraction import *
import mainWindow
import utils
import listView
import ingDialog

ID_BUTTON_ADD_INGREDIENT = wx.NewId()
ID_BUTTON_MOD_INGREDIENT = wx.NewId()
ID_BUTTON_DEL_INGREDIENT = wx.NewId()

# General Notes:
# - ListCtrl ID Column:
#    A keen observer may notice that most listCtrls have an ID column, which
# is set to 0 width. This is sort of a 'hack', but not 'really'. It fixes a few issues.
# Firstly, it ensures that each item in the listCtrl is unique. With the ID, we don't have
# to put any restrictions on the other fields. Secondly, it simplifies the modify/delete
# process. We instantly have the item clicked on, which gives us the unique ID, which
# allows us to edit the item in the database. Thirdly, there is a 'bug' while using images
# in the listCtrl column header (the small arrows seen when a column header is clicked on).
# The bug puts a small space to the left of every entry in the first column. By not
# even displaying the column, this bug cannot be noticed. The only time this 'hack' is
# revealed is if the user resizes the 'first' column - the ID column will be shown. This
# is not a big deal as most likely no one will ever realize this. If they do, it isn't
# a big deal either.

class AddModDialog(wx.Dialog):
    def __init__(self, parent, id, title, dbCon, mode, uniqueID = -1):
        wx.Dialog.__init__(self, parent, id, title, pos = wx.DefaultPosition,
                          size = (650, 450), style = wx.DEFAULT_DIALOG_STYLE)
        
        self.__mode = mode
        self.__con = dbCon
        self.__modified = False

        # When we click 'delete' for an ingredient, don't actually save to the database
        # unless we click "OK" when exiting the recipe dialog.
        self.__ingToDelete = []
        
        # A list of the ingredient columns to be displayed in the listCtrl
        self.__ingListCols = ['ingID', 'ingName', 'ingAmount', 'ingUnits', 'ingOptional', 'ingExtraNotes']
        
        self.__colIDWidth = 0
        self.__colNameWidth = 125
        self.__colAmountWidth = 75
        self.__colUnitsWidth = 90
        self.__colOptionalWidth = 90
        self.__colExtraNotesWidth = 100
        
        # Basic idea:
        # If we're adding a new recipe:
        #    Insert some junk into the recipe table in order to get a unique ID.
        #    If OK is clicked, just update this entry.
        #    If cancel is clicked, delete the junk entry created.
        # If we're modifying a recipe:
        #    We alrdy have a unique ID
        #    If OK is clicked, just update this entry.
        #    If cancel is clicked, do nothing (don't update).
        if self.__mode == "add":
            cur = self.__con.cursor()
            cur.execute("insert into " + mainWindow.gTableName + " (name) values ('')")

            self.__uniqueID = self.__getNewId(mainWindow.gTableName, "recipeID")
                    
            # Create the ingredient table.
            self.__ingTableName = utils.getIngTableName(self.__uniqueID)
            cur = self.__con.cursor()
            CREATE_TABLE = """create table """ + self.__ingTableName + """ (ingID INTEGER PRIMARY KEY,
                                    ingName, ingAmount, ingUnits, ingOptional, ingExtraNotes);"""
            cur.execute(CREATE_TABLE)
        elif self.__mode == "mod":
            self.__uniqueID = str(uniqueID)
            self.__ingTableName = utils.getIngTableName(self.__uniqueID)
            
        self.__buildLayout()
        
    def __buildLayout(self):
        categoryList = ['Any', 'Breakfast', 'Lunch', 'Dinner', 'Snack', 'Dessert',
            'Drink', 'Appetizer']
        
        cuisineList = ['American', 'Indian', 'Thai', 'Japanese', 'Chinese', 'Korean',
            'Slovak', 'Middle Eastern', 'Mexican', 'French',
            'Spanish', 'Italian']
        
        # If modify, add the alrdy existing entries to available choices.
        if self.__mode == "mod":
            self.__parseExistingFields(mainWindow.gTableName, "category", categoryList)
            self.__parseExistingFields(mainWindow.gTableName, "cuisine", cuisineList)

        topsizer = wx.BoxSizer(wx.VERTICAL)
        
        topPanelSizer = wx.BoxSizer(wx.VERTICAL)
        topPanel = wx.Panel(self, wx.ID_ANY)
        
        notebook = wx.Notebook(topPanel, wx.ID_ANY)
        topPanelSizer.Add(notebook, 1, wx.EXPAND | wx.ALIGN_CENTER)
        
        panel1 = wx.Panel(notebook, wx.ID_ANY)
        panel2 = wx.Panel(notebook, wx.ID_ANY)
        panel3 = wx.Panel(notebook, wx.ID_ANY)
        panel4 = wx.Panel(notebook, wx.ID_ANY)
        notebook.AddPage(panel1, "Description")
        notebook.AddPage(panel2, "Ingredients")
        notebook.AddPage(panel3, "Instructions")
        notebook.AddPage(panel4, "Extra Notes")
        
        # Page 1
        page1Sizer = wx.BoxSizer(wx.VERTICAL)
        
        txtName = wx.StaticText(panel1, wx.ID_ANY, "Name:")
        self.__txtName = wx.TextCtrl(panel1, wx.ID_ANY, "", size = (200, -1))
        sizer11 = wx.BoxSizer(wx.VERTICAL)
        sizer11.Add(txtName, 0)
        sizer11.Add(self.__txtName, 0, wx.SOUTH, 5)
        
        txtCategory = wx.StaticText(panel1, wx.ID_ANY, "Category:")
        self.__cbCategory = wx.ComboBox(panel1, wx.ID_ANY, "", size = (200, -1),
            choices = categoryList, style = wx.CB_DROPDOWN | wx.CB_SORT | wx.TE_PROCESS_ENTER)
        sizer12 = wx.BoxSizer(wx.VERTICAL)
        sizer12.Add(txtCategory, 0, wx.NORTH, 5)
        sizer12.Add(self.__cbCategory, 0, wx.SOUTH, 5)
        
        txtCuisine = wx.StaticText(panel1, wx.ID_ANY, "Cuisine:")
        self.__cbCuisine = wx.ComboBox(panel1, wx.ID_ANY, "", size = (200, -1),
            choices = cuisineList, style = wx.CB_DROPDOWN | wx.CB_SORT | wx.TE_PROCESS_ENTER)
        sizer13 = wx.BoxSizer(wx.VERTICAL)
        sizer13.Add(txtCuisine, 0, wx.NORTH, 5)
        sizer13.Add(self.__cbCuisine, 0, wx.SOUTH, 5)
        
        ratingList = ['', '1', '2', '3', '4', '5']
        txtRating = wx.StaticText(panel1, wx.ID_ANY, "Rating:")
        self.__choiceRating = wx.Choice(panel1, wx.ID_ANY, (50, 50), choices = ratingList)
        sizerRating = wx.BoxSizer(wx.VERTICAL)
        sizerRating.Add(txtRating, 0, wx.NORTH, 5)
        sizerRating.Add(self.__choiceRating, 0, wx.SOUTH, 5)
        
        txtPrepTime = wx.StaticText(panel1, wx.ID_ANY, "Prep. Time:")
        self.__txtPrepTime = wx.TextCtrl(panel1, wx.ID_ANY, "", size = (75, -1))
        sizerPrep = wx.BoxSizer(wx.VERTICAL)
        sizerPrep.Add(txtPrepTime, 0, wx.NORTH, 5)
        sizerPrep.Add(self.__txtPrepTime, 0, wx.SOUTH, 5)
        
        txtCookTime = wx.StaticText(panel1, wx.ID_ANY, "Cook Time:")
        self.__txtCookTime = wx.TextCtrl(panel1, wx.ID_ANY, "", size = (75, -1))
        sizerCook = wx.BoxSizer(wx.VERTICAL)
        sizerCook.Add(txtCookTime, 0, wx.NORTH, 5)
        sizerCook.Add(self.__txtCookTime, 0, wx.SOUTH, 5)
        
        sizer14 = wx.BoxSizer(wx.HORIZONTAL)
        sizer14.Add(sizerRating, 0, wx.EAST | wx.WEST, 15)
        sizer14.Add(sizerPrep, 0, wx.EAST | wx.WEST, 15)
        sizer14.Add(sizerCook, 0, wx.EAST | wx.WEST, 15)
        
        txtServings = wx.StaticText(panel1, wx.ID_ANY, "Servings:")
        self.__spinServings = wx.SpinCtrl(panel1, wx.ID_ANY, "", size = (60, -1))
        self.__spinServings.SetRange(0, 999)
        sizerServings = wx.BoxSizer(wx.VERTICAL)
        sizerServings.Add(txtServings, 0, wx.NORTH, 5)
        sizerServings.Add(self.__spinServings, wx.SOUTH, 5)
        
        txtSource = wx.StaticText(panel1, wx.ID_ANY, "Source:")
        self.__txtSource = wx.TextCtrl(panel1, wx.ID_ANY, "", size = (200, -1))
        sizerSource = wx.BoxSizer(wx.VERTICAL)
        sizerSource.Add(txtSource, 0, wx.NORTH, 5)
        sizerSource.Add(self.__txtSource, wx.SOUTH, 5)
        
        sizer15 = wx.BoxSizer(wx.HORIZONTAL)
        sizer15.Add(sizerServings, 0, wx.EAST | wx.WEST, 15)
        sizer15.Add(sizerSource, 0, wx.EAST | wx.WEST, 15)
            
        page1Sizer.Add(sizer11, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        page1Sizer.Add(sizer12, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        page1Sizer.Add(sizer13, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        page1Sizer.Add(sizer14, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        page1Sizer.Add(sizer15, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        
        panel1.SetSizer(page1Sizer)
        
        # Page 2
        page2Sizer = wx.BoxSizer(wx.VERTICAL)
        
        sizer21 = wx.BoxSizer(wx.HORIZONTAL)
        buttonAddIng = wx.Button(panel2, ID_BUTTON_ADD_INGREDIENT, "Add Ingredient")
        wx.EVT_BUTTON(self, ID_BUTTON_ADD_INGREDIENT, self.__onClickAddIng)
        buttonModIng = wx.Button(panel2, ID_BUTTON_MOD_INGREDIENT, "Modify Ingredient")
        wx.EVT_BUTTON(self, ID_BUTTON_MOD_INGREDIENT, self.__onClickModIng)
        buttonDelIng = wx.Button(panel2, ID_BUTTON_DEL_INGREDIENT, "Delete Ingredient")
        wx.EVT_BUTTON(self, ID_BUTTON_DEL_INGREDIENT, self.__onClickDelIng)
        sizer21.Add(buttonAddIng, 0, wx.EAST, 5)
        sizer21.Add(buttonModIng, 0, wx.EAST | wx.WEST, 5)
        sizer21.Add(buttonDelIng, 0, wx.WEST, 5)
        
        self.__ingList = listView.ListCtrl(panel2, wx.ID_ANY, 6,
                                        style = wx.LC_REPORT | wx.BORDER_NONE )#| wx.LC_HRULES | wx.LC_VRULES | wx.LC_SORT_ASCENDING)
        self.__ingList.InsertColumn(0, "ID")
        self.__ingList.InsertColumn(1, "Name")
        self.__ingList.InsertColumn(2, "Amt.")
        self.__ingList.SetColumnWidth(2, 50)
        self.__ingList.InsertColumn(3, "Units")
        self.__ingList.SetColumnWidth(3, 50)
        self.__ingList.InsertColumn(4, "Optional")
        self.__ingList.InsertColumn(5, "Extra Notes")

        page2Sizer.Add(sizer21, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        page2Sizer.Add(self.__ingList, 1, wx.EXPAND | wx.ALIGN_CENTER)
        
        panel2.SetSizer(page2Sizer)
        
        # Page 3
        page3Sizer = wx.BoxSizer(wx.VERTICAL)
        
        sizer31 = wx.BoxSizer(wx.VERTICAL)
        txtInst = wx.StaticText(panel3, wx.ID_ANY, "Add instructions below.",
                                style = wx.ALIGN_CENTER)
        self.__txtInst = wx.TextCtrl(panel3, wx.ID_ANY, "", size = wx.DefaultSize, style = wx.TE_MULTILINE)
        
        sizer31.Add(txtInst, 0, wx.SOUTH, 5)
        sizer31.Add(self.__txtInst, 1, wx.NORTH | wx.EXPAND, 5)
        
        page3Sizer.Add(sizer31, 1, wx.ALIGN_CENTER | wx.ALL | wx.EXPAND, 5)
        
        panel3.SetSizer(page3Sizer)
        
        # Page 4
        page4Sizer = wx.BoxSizer(wx.VERTICAL)
        
        sizer41 = wx.BoxSizer(wx.VERTICAL)
        txtNotes = wx.StaticText(panel4, wx.ID_ANY, "Add any additional notes below.")
        self.__txtNotes = wx.TextCtrl(panel4, wx.ID_ANY, "", size = wx.DefaultSize, style = wx.TE_MULTILINE)
        
        sizer41.Add(txtNotes, 0, wx.SOUTH, 5)
        sizer41.Add(self.__txtNotes, 1, wx.NORTH | wx.EXPAND, 5)
        
        page4Sizer.Add(sizer41, 1, wx.ALIGN_CENTER | wx.ALL | wx.EXPAND, 5)
        
        panel4.SetSizer(page4Sizer)
        
        # Buttons.
        buttonSizer = wx.BoxSizer(wx.HORIZONTAL)
        buttonOk = wx.Button(self, wx.ID_OK)
        wx.EVT_BUTTON(self, buttonOk.GetId(), self.OnOk)
        buttonCancel = wx.Button(self, wx.ID_CANCEL)
        wx.EVT_BUTTON(self, buttonCancel.GetId(), self.OnCancel)
        buttonSizer.Add(buttonOk, 0, wx.EAST, 5)
        buttonSizer.Add(buttonCancel, 0, wx.WEST, 5)
      
        topPanel.SetSizerAndFit(topPanelSizer)
        topsizer.Add(topPanel, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 20)
        topsizer.Add(buttonSizer, 0, wx.ALIGN_CENTER | wx.ALL, 20)
        
        self.SetSizer(topsizer)
        
        self.Bind(wx.EVT_TEXT, self.__onEvtTextName, self.__txtName)
        
        self.Bind(wx.EVT_COMBOBOX, self.__onEvtCBCategory, self.__cbCategory)
        self.Bind(wx.EVT_TEXT_ENTER, self.__onEvtTextEnterCategory, self.__cbCategory)
        
        self.Bind(wx.EVT_COMBOBOX, self.__onEvtCBCuisine, self.__cbCuisine)
        self.Bind(wx.EVT_TEXT_ENTER, self.__onEvtTextEnterCuisine, self.__cbCuisine)
        
        self.Bind(wx.EVT_SPINCTRL, self.__onEvtSpinServings, self.__spinServings)
        
        # Fill in existing fields.
        if self.__mode == "mod":
            self.__fillInValues()
            
        # Make it fit !
        self.__sizeColumns()

    # If OK is clicked, we want to make changes to the database.
    def OnOk(self, e):
        if self.__modified:
            cur = self.__con.cursor()
            entry = []
            entry.append(self.__txtName.GetLineText(0))
            entry.append(self.__cbCategory.GetValue())
            entry.append(self.__cbCuisine.GetValue())
            entry.append(self.__choiceRating.GetStringSelection())
            entry.append(self.__txtPrepTime.GetLineText(0))
            entry.append(self.__txtCookTime.GetLineText(0))
            entry.append(self.__txtSource.GetLineText(0))
            
            entry.append(self.__ingTableName)
            
            entry.append(self.__txtInst.GetValue())
            entry.append(self.__txtNotes.GetValue())
            
            entry.append(self.__spinServings.GetValue())
            
            cur.execute("""update """ + mainWindow.gTableName + 
                        """ set name = ?,
                                category = ?,
                                cuisine = ?,
                                rating = ?,
                                prepTime = ?,
                                cookTime = ?,
                                source = ?,
                                ingTableName = ?,
                                instructions = ?,
                                extraNotes = ?,
                                servings = ?
                         where recipeID = """ + self.__uniqueID, entry)
            
            # Update the ingredients deleted.
            for ing in self.__ingToDelete:
                cur.execute("delete from " + self.__ingTableName + " where ingID = " + ing)
        
        e.Skip()
        
    # If cancel is clicked, disregard all changes made.
    def OnCancel(self, e):
        # If we're adding a new entry, we created the temporary entry in the beginning (for a unique ID),
        # so we now need to delete it.
        if self.__mode == "add":
            cur = self.__con.cursor()
            cur.execute("delete from " + mainWindow.gTableName + " where recipeID = " + self.__uniqueID)
            
            # Also delete the created ingredient table.
            cur.execute("drop table " + self.__ingTableName)
            
        e.Skip()
        
    # Updates the ingredient listCtrl based on the status of the database.        
    def __updateIngList(self):
        cur = self.__con.cursor()
        
        # Comma between each column name
        colsStr = ','.join(self.__ingListCols)

        # Select the table
        cur.execute("select " + colsStr + " from " + self.__ingTableName + " order by ingName desc")
        
        self.__ingList.DeleteAllItems()
        
        # For the sorter to work, we must create a dictionary.
        itemDict = {}
        i = 0
        for row in cur:
            aTuple = (row[0], row[1], row[2], row[3], row[4], row[5]) # 6 list columns displayed
            itemDict[i] = aTuple
            i += 1
            
        # Needed for the sorter mixin.
        self.__ingList.itemDataMap = itemDict
        
        # Add the items to the ListCtrl
        items = itemDict.items()
        for key, data in items:
            data = utils.eliminateNones(data)
            
            index = self.__ingList.InsertStringItem(sys.maxint, str(data[0]))
            self.__ingList.SetStringItem(index, 1, data[1])
            self.__ingList.SetStringItem(index, 2, data[2])
            self.__ingList.SetStringItem(index, 3, data[3])
            self.__ingList.SetStringItem(index, 4, utils.convertOptionalBool(data[4]))
            self.__ingList.SetStringItem(index, 5, data[5])
            self.__ingList.SetItemData(index, key)
            
        self.__sizeColumns()
     
        # Must call this to get ListCtrl to span entire width
        self.__ingList.resizeLastColumn(100)
        
        # Resort list
        self.__ingList.SortListItems()
        
    # Add a new ingredient.
    def __onClickAddIng(self, e):
        dlg = ingDialog.IngredientDialog(self, wx.ID_ANY, "Add New Ingredient")
                
        if dlg.ShowModal() == wx.ID_OK:
            fields = dlg.getFields()
            
            # Insert to ingredient table (in database).
            entry = []
            entry.append(fields[0])
            entry.append(str(toFraction(fields[1], fields[2], fields[3])))
            entry.append(fields[4])
            entry.append(fields[5])
            entry.append(fields[6])
            
            cur = self.__con.cursor()
            cur.execute("""insert into """ + self.__ingTableName +
                         """        (ingName, ingAmount, ingUnits, ingOptional, ingExtraNotes)
                           values 
                                    (?, ?, ?, ?, ?)""",
                           entry)
            
            self.__updateIngList()
            
            self.__modified = True
        
        dlg.Destroy()

    # Modify an existing ingredient.
    def __onClickModIng(self, e):
        # Verify 1 item is selected
        if self.__ingList.GetSelectedItemCount() > 1 or self.__ingList.GetSelectedItemCount() == 0:
            errDlg = wx.MessageDialog(self, "You must select exactly 1 item to modify.", "Invalid Selection", wx.OK | wx.ICON_ERROR)
            errDlg.ShowModal()
            errDlg.Destroy()
            return
            
        # Pass existing data into dialog (skip first column which is the ID)
        data = []
        index = self.__ingList.GetFirstSelected()
        for i in range(1, self.__ingList.GetColumnCount()):
            item = self.__ingList.GetItem(index, i)
            data.append(item.GetText())
            
        dlg = ingDialog.IngredientDialog(self, wx.ID_ANY, "Modify Ingredient")
        dlg.setFields(data)
                
        if dlg.ShowModal() == wx.ID_OK:
            ingID = self.__ingList.GetItem(index, 0).GetText()

            # Update the database.
            fields = dlg.getFields()
            
            entry = []
            entry.append(fields[0])
            entry.append(str(toFraction(fields[1], fields[2], fields[3])))
            entry.append(fields[4])
            entry.append(fields[5])
            entry.append(fields[6])            
            
            cur = self.__con.cursor()
            cur.execute("""update """ + self.__ingTableName +
                        """ set ingName = ?,
                                ingAmount = ?,
                                ingUnits = ?,
                                ingOptional = ?,
                                ingExtraNotes = ?
                            where ingID = """ + ingID, entry)                 
            
            self.__updateIngList()
            
            self.__modified = True
        
        dlg.Destroy()
    
    # Deleted the selected ingredient(s).
    def __onClickDelIng(self, e):
        # Verify items are selected
        if self.__ingList.GetSelectedItemCount() == 0:
            errDlg = wx.MessageDialog(self, "No items selected to delete.", "Invalid Selection", wx.OK | wx.ICON_ERROR)
            errDlg.ShowModal()
            errDlg.Destroy()
            return
        
        retCode = wx.MessageBox("Are you sure you want to delete the selected ingredients?", "Delete Confirmation", wx.YES_NO | wx.ICON_QUESTION, self)
        if retCode != wx.YES:
            return
        
        self.__modified = True
        
        # For all selected ingredients...
        while True:
            index = self.__ingList.GetFirstSelected()
            if index == -1:
                break
            
            # Don't update the database here. Wait till they click "OK"
            item = self.__ingList.GetItem(index, 0)
            id = item.GetText()
            self.__ingToDelete.append(id)
            
            # Update the list view.
            self.__ingList.DeleteItem(index)

    # Resize all columns to their pre-specified widths.
    def __sizeColumns(self):
        self.__ingList.SetColumnWidth(0, self.__colIDWidth)
        self.__ingList.SetColumnWidth(1, self.__colNameWidth)
        self.__ingList.SetColumnWidth(2, self.__colAmountWidth)
        self.__ingList.SetColumnWidth(3, self.__colUnitsWidth)
        self.__ingList.SetColumnWidth(4, self.__colOptionalWidth)
        self.__ingList.SetColumnWidth(5, self.__colExtraNotesWidth)

    def __onEvtTextName(self, e):
        self.__modified = True
        
    def __onEvtCBCategory(self, e):
        self.__modified = True
    
    def __onEvtTextEnterCategory(self, e):
        txt = e.GetString()
        
        # Only add if string doesn't alrdy exist.
        idx = self.__cbCategory.FindString(txt)
        if idx == -1:
            idx = self.__cbCategory.Append(txt)
        self.__cbCategory.SetSelection(idx)
        
        self.__modified = True
    
    def __onEvtCBCuisine(self, e):
        self.__modified = True
     
    def __onEvtTextEnterCuisine(self, e):
        txt = e.GetString()
        
        # Only add if string doesn't alrdy exist.
        idx = self.__cbCuisine.FindString(txt)
        if idx == -1:
            idx = self.__cbCuisine.Append(txt)
        self.__cbCuisine.SetSelection(idx)
        
        self.__modified = True
        
    def __onEvtSpinServings(self, e):
        self.__modified = True
        
    # If 'custom' fields exist in the fieldStr column, add these to aList if they don't
    # alrdy exist.
    def __parseExistingFields(self, table, fieldStr, aList):
        cur = self.__con.cursor()
        cur.execute("select " + fieldStr + " from " + table)
        
        entry = cur.fetchone()
                
        # For all items in the current database...
        while entry != None:
            
            # Beware of blanks and Nones. And if not in our list alrdy, add it.
            if entry[0] != "" and entry[0] != None and self.__find(entry[0], aList) == False:
                aList.append(entry[0])
                
            # Get the next item
            entry = cur.fetchone()
        
    # Finds an item in a list. Probably a better (Python) way to do this.
    # Returns true if found, False if not.
    def __find(self, item, list):
        for i in list:
            if i == item:
                return True
        return False
        
    # Fills in fields w/ values from database (used when recipe is modified)
    def __fillInValues(self):
        cur = self.__con.cursor()
        cur.execute("select * from " + mainWindow.gTableName + " where recipeID = " + self.__uniqueID)
            
        entry = cur.fetchone()
        entry = utils.eliminateNones(entry)
            
        self.__txtName.SetValue(entry[1])
            
        self.__cbCategory.SetValue(entry[2])
        self.__cbCategory.SetStringSelection(entry[2])
            
        self.__cbCuisine.SetValue(entry[3])
        self.__cbCuisine.SetStringSelection(entry[3])
            
        self.__choiceRating.SetStringSelection(entry[4])
            
        self.__txtPrepTime.SetValue(entry[5])
            
        self.__txtCookTime.SetValue(entry[6])
            
        self.__txtSource.SetValue(entry[7])
            
        self.__updateIngList()
            
        self.__txtInst.SetValue(entry[9])
            
        self.__txtNotes.SetValue(entry[10])
        
        # If no servings specified, default to 0.
        spinEntry = entry[11]
        if spinEntry == "" or spinEntry == None: spinEntry = 0
        self.__spinServings.SetValue(int(spinEntry))

    # Given a table name and the id column name, this returns the max id (aka the 
    # last added id) in string form. Not safe if you will have many users in the system
    # at once... but that will never happen here.
    def __getNewId(self, tableName, idName):
        cur = self.__con.cursor()
        cur.execute("select max(" + str(idName) + ") from " + str(tableName))
        rowTuple = cur.fetchone()
        return str(rowTuple[0])