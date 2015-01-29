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
from pysqlite2 import dbapi2 as sqlite

import mainWindow
import addModDialog
import utils
import listView
import stringSearch

ID_BUTTON_CLEAR_TXT_SEARCH = wx.NewId()
        
class SearchPanel(wx.Panel):
    def __init__(self, parent, parentWindow):
        wx.Panel.__init__(self, parent, wx.ID_ANY, pos = wx.DefaultPosition,
            size = wx.DefaultSize, style = 0)
        
        self.__parentWindow = parentWindow
        
        topsizer = wx.BoxSizer(wx.VERTICAL)
        
        searchSizer = wx.BoxSizer(wx.HORIZONTAL)
        statSearchFor = wx.StaticText(self, wx.ID_ANY, "Search for:")
        self.__txtSearchBox = wx.TextCtrl(self, wx.ID_ANY, "", size = (150, -1))
        statSearchIn = wx.StaticText(self, wx.ID_ANY, "in the field:") 
        
        choiceList = ['Recipe Name', 'Category', 'Cuisine', 'Source', 'Ingredient Name']
        self.__choiceField = wx.Choice(self, wx.ID_ANY, (100, 50), choices = choiceList)
        self.__choiceField.SetSelection(0)
        
        buttonClearTxtSearch = wx.Button(self, ID_BUTTON_CLEAR_TXT_SEARCH, "Clear Search")
        
        self.__checkCaseSens = wx.CheckBox(self, wx.ID_ANY, "Case Sensitive")
        
        searchSizer.Add(statSearchFor, 0, wx.EAST | wx.ALIGN_CENTRE_VERTICAL, 5)
        searchSizer.Add(self.__txtSearchBox, 0, wx.WEST | wx.EAST | wx.ALIGN_CENTRE_VERTICAL, 5)
        searchSizer.Add(statSearchIn, 0, wx.WEST | wx.EAST | wx.ALIGN_CENTRE_VERTICAL, 5)
        searchSizer.Add(self.__choiceField, 0, wx.WEST | wx.EAST | wx.ALIGN_CENTRE_VERTICAL, 5)
        searchSizer.Add(buttonClearTxtSearch, 0, wx.WEST | wx.EAST | wx.ALIGN_CENTRE_VERTICAL, 5)
        searchSizer.Add(self.__checkCaseSens, 0, wx.WEST | wx.ALIGN_CENTRE_VERTICAL, 15)
        
        self.__txtSearchBox.Bind(wx.EVT_TEXT, self.__onEvtTxtSearch)
        self.__choiceField.Bind(wx.EVT_CHOICE, self.__onEvtChoiceField)
        wx.EVT_BUTTON(self, ID_BUTTON_CLEAR_TXT_SEARCH, self.__onClickClearTxtSearch)
        self.__checkCaseSens.Bind(wx.EVT_CHECKBOX, self.__onCheckCaseSense)
        
        self.__searchList = listView.ListCtrl(self, wx.ID_ANY, 8,
            style = wx.LC_REPORT | wx.LC_HRULES | wx.LC_VRULES | wx.LC_SORT_ASCENDING)
        
        self.__searchList.InsertColumn(0, "ID")
        self.__searchList.InsertColumn(1, "Name")
        self.__searchList.InsertColumn(2, "Category")
        self.__searchList.InsertColumn(3, "Cuisine")
        self.__searchList.InsertColumn(4, "Rating")
        self.__searchList.InsertColumn(5, "Prep. Time")
        self.__searchList.InsertColumn(6, "Cook Time")
        self.__searchList.InsertColumn(7, "Source")
        
        self.__colIDWidth = 0
        self.__colNameWidth = 125
        self.__colCatWidth = 100
        self.__colCuiWidth = 100
        self.__colRatWidth = 85
        self.__colPrepWidth = 100
        self.__colCookWidth = 100
        self.__colSourWidth = 100
            
        topsizer.Add(searchSizer, 0, wx.ALL | wx.ALIGN_LEFT, 5)
        topsizer.Add(self.__searchList, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 5)
        
        self.SetSizerAndFit(topsizer)
        
        # Make it fit !
        self.__sizeColumns()

    # Updates the entire list w/ the list currently stored in the database.
    # It will delete all items currently displayed, and add whatever items are allowed
    # given the current filter specifications.
    def update(self, con):
        cur = ""
        try:
            cur = con.cursor()
        except sqlite.ProgrammingError:
            return
        
        # Comma between each column name
        tableCols = mainWindow.gTableCols + ['ingTableName']
        colsStr = ','.join(tableCols)
        
        # Select the table
        cur.execute("select " + colsStr + " from " + mainWindow.gTableName + " order by name desc")
        
        self.__searchList.DeleteAllItems()
        
        # Match the currently selected field to it's number in the database (mainWindow.gTableCols)
        # In the returned cursor, 0 - 7 will be the fields displayed in the listCtrl, and 8 will be
        # the ingredient table name (for easy access).
        filterCat = self.__choiceField.GetStringSelection()
        filterCatNum = 0
        if filterCat == "Recipe Name":
            filterCatNum = 1
        elif filterCat == "Category":
            filterCatNum = 2
        elif filterCat == "Cuisine":
            filterCatNum = 3
        elif filterCat == "Source":
            filterCatNum = 7
        elif filterCat == "Ingredient Name":
            filterCatNum = 8
        
        caseSensitiveValue = self.__checkCaseSens.GetValue()
        
        # Compute the shifts once only.
        filter = self.__txtSearchBox.GetValue()
        if caseSensitiveValue == False:
            filter = filter.lower()
        kmpShifts = stringSearch.KMPComputeShifts(filter)
        
        # For the sorter to work, we must create a dictionary.
        itemDict = {}
        i = 0
        
        # For each item in the database.
        for row in cur:
        
            # We need to parse the ingredient table.
            if filterCatNum == 8:
                ingCur = con.cursor()
                ingCur.execute("select ingName from " + row[filterCatNum])
                
                alrdyHandled = False
                
                # For each ingredient this recipe contains.
                for ingRow in ingCur:
                    alrdyHandled = True
                    
                    # ingName column
                    txt = ingRow[0]
                    if caseSensitiveValue == False:
                        txt = txt.lower()
                        
                    # If match found, add to listCtrl. Break out - one ingredient matched; no need to check others.
                    if stringSearch.KMPStringSearchWithShifts(filter, txt, kmpShifts):
                        aTuple = (row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]) # 8 ListCtrl columns displayed
                        itemDict[i] = aTuple
                        i += 1
                        break
                
                # Handle recipes with no ingredients (they will never enter the above loop).
                if alrdyHandled == False and ingCur.fetchall() == []:
                    txt = ""
                    if stringSearch.KMPStringSearchWithShifts(filter, txt, kmpShifts):
                        aTuple = (row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]) # 8 ListCtrl columns displayed
                        itemDict[i] = aTuple
                        i += 1
            
            # We just need to parse the standard recipe table.
            else:
                txt = row[filterCatNum]
                if caseSensitiveValue == False:
                    txt = txt.lower()
                
                # If match found, add to listCtrl.
                if stringSearch.KMPStringSearchWithShifts(filter, txt, kmpShifts):
                    aTuple = (row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]) # 8 ListCtrl columns displayed
                    itemDict[i] = aTuple
                    i += 1

        # Needed for the sorter mixin.
        self.__searchList.itemDataMap = itemDict
        
        # Add the items to the ListCtrl
        items = itemDict.items()
        for key, data in items:
            data = utils.eliminateNones(data)
            
            index = self.__searchList.InsertStringItem(sys.maxint, str(data[0]))
            self.__searchList.SetStringItem(index, 1, data[1])
            self.__searchList.SetStringItem(index, 2, data[2])
            self.__searchList.SetStringItem(index, 3, data[3])
            self.__searchList.SetStringItem(index, 4, utils.ratingFormated(data[4], False))
            self.__searchList.SetStringItem(index, 5, data[5])
            self.__searchList.SetStringItem(index, 6, data[6])
            self.__searchList.SetStringItem(index, 7, data[7])
            self.__searchList.SetItemData(index, key)
            
        self.__sizeColumns()
        
        # Must call this to get ListCtrl to span entire width
        self.__searchList.resizeLastColumn(100)
        
        # Resort list
        self.__searchList.SortListItems()
    
    # Resize all columns to their pre-specified widths.
    def __sizeColumns(self):
        self.__searchList.SetColumnWidth(0, self.__colIDWidth)
        self.__searchList.SetColumnWidth(1, self.__colNameWidth)
        self.__searchList.SetColumnWidth(2, self.__colCatWidth)
        self.__searchList.SetColumnWidth(3, self.__colCuiWidth)
        self.__searchList.SetColumnWidth(4, self.__colRatWidth)
        self.__searchList.SetColumnWidth(5, self.__colPrepWidth)
        self.__searchList.SetColumnWidth(6, self.__colCookWidth)
        self.__searchList.SetColumnWidth(7, self.__colSourWidth)
     
    def getNumSelected(self):
        return self.__searchList.GetSelectedItemCount()
    
    def getFirstSelected(self):
        return self.__searchList.GetFirstSelected()
    
    def getNextSelected(self, index):
        return self.__searchList.GetNextSelected(index)
    
    def deleteSelected(self, con):
        while True:
            index = self.__searchList.GetFirstSelected()
            if index == -1:
                break
            
            # Update the database.
            item = self.__searchList.GetItem(index, 0)
            id = item.GetText()
            cur = con.cursor()
            cur.execute("delete from " + mainWindow.gTableName + " where recipeID = " + id)
            
            # Also delete it's ingredients! (drop the entire table)
            cur.execute("drop table " + utils.getIngTableName(id))
            
            # Update the list view.
            self.__searchList.DeleteItem(index)
            
    def clearAll(self):
        self.__searchList.DeleteAllItems()
        
        # Must set selection before clearing txt. SetSelection() does not trigger
        # the choice callback. Clear() triggers the Txt callback. So, for update to 
        # behave properly, we must do it in this order.
        self.__choiceField.SetSelection(0)
        self.__txtSearchBox.Clear()
        
    # Return a list of uniqueIDs of all selected items in the list
    def getUniqueID(self):
        selectedIDs = []
        numSelected = self.getNumSelected()
        
        index = self.getFirstSelected()
        for i in range(0, numSelected):
            item = self.__searchList.GetItem(index, 0)    # ID is always first column
            selectedIDs.append(item.GetText())
            index = self.getNextSelected(index)
        
        return selectedIDs
        
    def __onEvtTxtSearch(self, e):
        # If no database is loaded, there's nothing to do.
        if self.__parentWindow.getIsDBLoaded() == False:
            return

        self.update(self.__parentWindow.getConnection())
        
    def __onEvtChoiceField(self, e):
        # If no database is loaded, there's nothing to do.
        if self.__parentWindow.getIsDBLoaded() == False:
            return
        
        self.update(self.__parentWindow.getConnection())
        
    def __onCheckCaseSense(self, e):
        # If no database is loaded, there's nothing to do.
        if self.__parentWindow.getIsDBLoaded() == False:
            return
        
        self.update(self.__parentWindow.getConnection())
        
    def __onClickClearTxtSearch(self, e):
        # Must set selection before clearing txt. SetSelection() does not trigger
        # the choice callback. Clear() triggers the Txt callback. So, for update to 
        # behave properly, we must do it in this order.
        self.__choiceField.SetSelection(0)
        self.__checkCaseSens.SetValue(False)
        self.__txtSearchBox.Clear()