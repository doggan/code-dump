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

import wx
import sys

from fraction import *
import mainWindow
import listView
import utils
import printer

ID_BUTTON_PRINT = wx.NewId()
ID_BUTTON_PRINT_PREVIEW = wx.NewId()

ID_BUTTON_CLOSE = wx.NewId()

ID_BUTTON_RESET_SERVINGS = wx.NewId()

class ViewDialog(wx.Dialog):
    def __init__(self, parent, id, title, dbCon, uniqueID):
        wx.Dialog.__init__(self, parent, id | wx.ID_ANY, title, pos = wx.DefaultPosition,
                          size = wx.DefaultSize, style = wx.DEFAULT_DIALOG_STYLE)
        
        self.__con = dbCon
        self.__uniqueID = uniqueID
        self.__ingTableName = utils.getIngTableName(self.__uniqueID)
        
        # Create the printer.
        self.__printer = printer.Printer()
        
        # A list of the ingredient columns to be displayed in the listCtrl
        self.__ingListCols = ['ingName', 'ingAmount', 'ingUnits', 'ingOptional', 'ingExtraNotes']
        
        self.__colBlankWidth = 0
        self.__colNameWidth = 115
        self.__colAmountWidth = 75
        self.__colUnitsWidth = 90
        self.__colOptionalWidth = 90
        self.__colExtraNotesWidth = 100
        
        self.__buildLayout()
        self.__fillInValues()
        
    def __buildLayout(self):
        topsizer = wx.BoxSizer(wx.VERTICAL)
        
        row1 = wx.BoxSizer(wx.HORIZONTAL)
        
        txtName = wx.StaticText(self, wx.ID_ANY, "Name:")
        self.__txtName = wx.TextCtrl(self, wx.ID_ANY, "", size = (150, -1), style = wx.TE_READONLY)
        nameSizer = wx.BoxSizer(wx.VERTICAL)
        nameSizer.Add(txtName, 0)
        nameSizer.Add(self.__txtName, 0)
        
        txtCategory = wx.StaticText(self, wx.ID_ANY, "Category:")
        self.__txtCategory = wx.TextCtrl(self, wx.ID_ANY, "", size = (150, -1), style = wx.TE_READONLY)
        catSizer = wx.BoxSizer(wx.VERTICAL)
        catSizer.Add(txtCategory, 0)
        catSizer.Add(self.__txtCategory, 0)
        
        txtCuisine = wx.StaticText(self, wx.ID_ANY, "Cuisine:")
        self.__txtCuisine = wx.TextCtrl(self, wx.ID_ANY, "", size = (150, -1), style = wx.TE_READONLY)
        cuiSizer = wx.BoxSizer(wx.VERTICAL)
        cuiSizer.Add(txtCuisine, 0)
        cuiSizer.Add(self.__txtCuisine, 0)
        
        row1.Add(nameSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row1.Add(catSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row1.Add(cuiSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        
        row2 = wx.BoxSizer(wx.HORIZONTAL)
        
        txtRating = wx.StaticText(self, wx.ID_ANY, "Rating:")
        self.__txtRating = wx.TextCtrl(self, wx.ID_ANY, "", size = (50, -1), style = wx.TE_READONLY)
        ratingSizer = wx.BoxSizer(wx.VERTICAL)
        ratingSizer.Add(txtRating, 0)
        ratingSizer.Add(self.__txtRating, 0)
        
        txtPrep = wx.StaticText(self, wx.ID_ANY, "Prep. Time:")
        self.__txtPrep = wx.TextCtrl(self, wx.ID_ANY, "", size = (100, -1), style = wx.TE_READONLY)
        prepSizer = wx.BoxSizer(wx.VERTICAL)
        prepSizer.Add(txtPrep, 0)
        prepSizer.Add(self.__txtPrep, 0)
        
        txtCook = wx.StaticText(self, wx.ID_ANY, "Cook Time:")
        self.__txtCook = wx.TextCtrl(self, wx.ID_ANY, "", size = (100, -1), style = wx.TE_READONLY)
        cookSizer = wx.BoxSizer(wx.VERTICAL)
        cookSizer.Add(txtCook, 0)
        cookSizer.Add(self.__txtCook, 0)
        
        txtSource = wx.StaticText(self, wx.ID_ANY, "Source:")
        self.__txtSource = wx.TextCtrl(self, wx.ID_ANY, "", size = (125, -1), style = wx.TE_READONLY)
        sourceSizer = wx.BoxSizer(wx.VERTICAL)
        sourceSizer.Add(txtSource, 0)
        sourceSizer.Add(self.__txtSource, 0)
    
        row2.Add(ratingSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row2.Add(prepSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row2.Add(cookSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row2.Add(sourceSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        
        row3 = wx.BoxSizer(wx.HORIZONTAL)
        
        txtServings = wx.StaticText(self, wx.ID_ANY, "Servings:")
        self.__spinServings = wx.SpinCtrl(self, wx.ID_ANY, "", size = (60, -1))
        self.__spinServings.SetRange(1, 999)
        servingsSizer = wx.BoxSizer(wx.VERTICAL)
        servingsSizer.Add(txtServings, 0)
        servingsSizer.Add(self.__spinServings, 0)
        
        self.Bind(wx.EVT_SPINCTRL, self.__onEvtSpinServings, self.__spinServings)
        
        buttonResetServ = wx.Button(self, ID_BUTTON_RESET_SERVINGS, "Reset Servings")
        wx.EVT_BUTTON(self, ID_BUTTON_RESET_SERVINGS, self.__onClickResetServings)
        
        row3.Add(servingsSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        row3.Add(buttonResetServ, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER | wx.ALIGN_BOTTOM, 5)
        
        row4 = wx.BoxSizer(wx.HORIZONTAL)
        
        txtIngredients = wx.StaticText(self, wx.ID_ANY, "Ingredients:")
        self.__ingList = listView.ListCtrl(self, wx.ID_ANY, 5, size = (525, 125),
                                           style = wx.LC_REPORT | wx.LC_HRULES | wx.LC_VRULES | wx.LC_SORT_ASCENDING)
        self.__ingList.InsertColumn(0, "")
        self.__ingList.InsertColumn(1, "Name")
        self.__ingList.InsertColumn(2, "Amt.")
        self.__ingList.SetColumnWidth(2, 50)
        self.__ingList.InsertColumn(3, "Units")
        self.__ingList.SetColumnWidth(3, 50)
        self.__ingList.InsertColumn(4, "Optional")
        self.__ingList.InsertColumn(5, "Extra Notes")
        ingSizer = wx.BoxSizer(wx.VERTICAL)
        ingSizer.Add(txtIngredients, 0)
        ingSizer.Add(self.__ingList, 0)
        
        row4.Add(ingSizer, 0, wx.EAST | wx.WEST | wx.ALIGN_CENTER, 5)
        
        row5 = wx.BoxSizer(wx.HORIZONTAL)
        
        txtInst = wx.StaticText(self, wx.ID_ANY, "Instructions:")
        self.__txtInst = wx.TextCtrl(self, wx.ID_ANY, "", size = (200, 150), style = wx.TE_READONLY | wx.TE_MULTILINE)
        instSizer = wx.BoxSizer(wx.VERTICAL)
        instSizer.Add(txtInst, 0)
        instSizer.Add(self.__txtInst, 1, wx.EXPAND)
        
        txtNotes = wx.StaticText(self, wx.ID_ANY, "Extra Notes:")
        self.__txtNotes = wx.TextCtrl(self, wx.ID_ANY, "", size = (200, 150), style = wx.TE_READONLY | wx.TE_MULTILINE)
        notesSizer = wx.BoxSizer(wx.VERTICAL)
        notesSizer.Add(txtNotes, 0)
        notesSizer.Add(self.__txtNotes, 1, wx.EXPAND)
        
        row5.Add(instSizer, 1, wx.EAST | wx.WEST | wx.ALIGN_CENTER | wx.EXPAND, 5)
        row5.Add(notesSizer, 1, wx.EAST | wx.WEST | wx.ALIGN_CENTER | wx.EXPAND, 5)

        # Buttons
        buttonPrint = wx.Button(self, ID_BUTTON_PRINT, "Print")
        wx.EVT_BUTTON(self, ID_BUTTON_PRINT, self.__onClickPrint)
        buttonPrintPreview = wx.Button(self, ID_BUTTON_PRINT_PREVIEW, "Print Preview")
        wx.EVT_BUTTON(self, ID_BUTTON_PRINT_PREVIEW, self.__onClickPrintPreview)
        buttonSizer = wx.BoxSizer(wx.HORIZONTAL)
        buttonSizer.Add(buttonPrint, 0, wx.ALL, 5)
        buttonSizer.Add(buttonPrintPreview, 0, wx.ALL, 5)
        
        buttonClose = wx.Button(self, ID_BUTTON_CLOSE, "Close")
        wx.EVT_BUTTON(self, ID_BUTTON_CLOSE, self.__onClickClose)
        
        topsizer.Add(row1, 0, wx.ALL | wx.ALIGN_CENTER, 10)
        topsizer.Add(row2, 0, wx.ALL | wx.ALIGN_CENTER, 10)
        topsizer.Add(row3, 0, wx.ALL | wx.ALIGN_CENTER, 10)
        topsizer.Add(row4, 0, wx.ALL | wx.ALIGN_CENTER, 10)
        topsizer.Add(row5, 1, wx.ALL | wx.ALIGN_CENTER | wx.EXPAND, 10)
        topsizer.Add(buttonSizer, 0, wx.NORTH | wx.ALIGN_CENTER, 20)
        topsizer.Add(buttonClose, 0, wx.SOUTH | wx.ALIGN_CENTER, 20)
        
        self.SetSizerAndFit(topsizer)
    
    # Read the values from the database and fill them into the various dialog fields.
    def __fillInValues(self):
        cur = self.__con.cursor()
        cur.execute("select * from " + mainWindow.gTableName + " where recipeID = " + self.__uniqueID + " order by name desc")
            
        entry = cur.fetchone()
        entry = utils.eliminateNones(entry)
            
        self.__txtName.SetValue(entry[1])
        self.__txtCategory.SetValue(entry[2])
        self.__txtCuisine.SetValue(entry[3])
        
        ratValue = entry[4]
        if ratValue == "":
            self.__txtRating.SetValue(ratValue)
        else:
            self.__txtRating.SetValue(ratValue + "/5")
            
        self.__txtPrep.SetValue(entry[5])
        self.__txtCook.SetValue(entry[6])
        self.__txtSource.SetValue(entry[7])
        self.__updateIngList()
        self.__txtInst.SetValue(entry[9])
        self.__txtNotes.SetValue(entry[10])
        
        # Save this so we can revert back later.
        self.__originalServings = int(entry[11])
        self.__prevServings = self.__originalServings
        
        # We can't allow amount modification if we don't know how many servings this is for.
        if self.__originalServings == 0:
            self.__spinServings.Enable(False)
            self.__spinServings.SetRange(0, 999)
            
        self.__spinServings.SetValue(int(entry[11]))
        
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
            aTuple = ('', row[0], row[1], row[2], row[3], row[4]) # 6 list columns displayed + blank column
            itemDict[i] = aTuple
            i += 1
            
        # Needed for the sorter mixin.
        self.__ingList.itemDataMap = itemDict
        
        # Add the items to the ListCtrl
        items = itemDict.items()
        for key, data in items:
            data = utils.eliminateNones(data)
            
            index = self.__ingList.InsertStringItem(sys.maxint, data[0])
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
    
    # Resize all columns to their pre-specified widths.
    def __sizeColumns(self):
        self.__ingList.SetColumnWidth(0, self.__colBlankWidth)
        self.__ingList.SetColumnWidth(1, self.__colNameWidth)
        self.__ingList.SetColumnWidth(2, self.__colAmountWidth)
        self.__ingList.SetColumnWidth(3, self.__colUnitsWidth)
        self.__ingList.SetColumnWidth(4, self.__colOptionalWidth)
        self.__ingList.SetColumnWidth(5, self.__colExtraNotesWidth)
    
    def __onClickResetServings(self, e):
        # Reset actual spinner.
        self.__spinServings.SetValue(self.__originalServings)
        
        # Also reset original amount values.
        self.__onEvtSpinServings(e)
        
    def __onClickPrint(self, e):
        self.__printer.Print(self.__fieldsToHTML())
        
    def __onClickPrintPreview(self, e):
        self.__printer.PreviewText(self.__fieldsToHTML())
        
    def __onClickClose(self, e):
        self.Destroy()
        
    # Servings spinner has changed value. Use this new value to adjust ingredient amounts.
    def __onEvtSpinServings(self, e):
        newValue = self.__spinServings.GetValue()
        prevValue = self.__prevServings
        
        multiplier = toFraction(0, newValue, prevValue)
        
        # For each item in the ListCtrl
        for index in range(0, self.__ingList.GetItemCount()):
            # The amount's column index in the ListCtrl.
            amtIndex = 2
            
            # Get it's current amount
            item = self.__ingList.GetItem(index, amtIndex)
            amtStr = item.GetText()
            curAmt = toFractionParse(amtStr)
            
            newAmt = curAmt * multiplier
            
            self.__ingList.SetStringItem(index, amtIndex, newAmt.toString())
        
        self.__prevServings = newValue
        
    # Reads in all current values of the fields. Returns a nice HTML formatted string
    # which can be printed easily.
    def __fieldsToHTML(self):        
        nameStr = utils.blankToDash(self.__txtName.GetValue())
        catStr = utils.blankToDash(self.__txtCategory.GetValue())
        cuiStr = utils.blankToDash(self.__txtCuisine.GetValue())
        prepStr = utils.blankToDash(self.__txtPrep.GetValue())
        cookStr = utils.blankToDash(self.__txtCook.GetValue())
        
        ratValue = self.__txtRating.GetValue()
        ratingStr = ""
        if ratValue == "":
            ratingStr = "-"
        else:
            ratingStr = utils.ratingFormated(ratValue[0], True)
        
        srcStr = utils.blankToDash(self.__txtSource.GetValue())
        servStr = str(self.__spinServings.GetValue())
        
        # HTML format our ingredient list
        ingStr = ""
        for index in range(0, self.__ingList.GetItemCount()):
            cols = (self.__ingList.GetItem(index, 1).GetText(),    # Name
                    self.__ingList.GetItem(index, 2).GetText(),    # Amount
                    self.__ingList.GetItem(index, 3).GetText(),    # Units
                    self.__ingList.GetItem(index, 4).GetText(),    # Optional
                    self.__ingList.GetItem(index, 5).GetText()     # Extra Notes
                    )
            
            optStr = ""
            if cols[3] == "Not Required":
                optStr = "*"
            extraNoteStr = "(" + cols[4] + ")"
            if cols[4] == "":
                extraNoteStr = ""
            
            # Of the form: 3 5/8 cup salt <i>(extra notes here)</i><br>
            ingStr = ingStr[:] + optStr + cols[1] + " " + cols[2] + " " + cols[0] + """ <i>""" + extraNoteStr + """</i><br>"""
        
        # These are multi-line texts, so we must parse accordingly.
        instStr = utils.blankToDash(self.__txtInst.GetValue())
        instStr = instStr.replace('\n', '<br>')
        
        # Ditto.
        notesStr = utils.blankToDash(self.__txtNotes.GetValue())
        notesStr = notesStr.replace('\n', '<br>')
        
        text = """<font face="verdana" size=+2>""" + nameStr + """</font><br><br>
            <font face="verdana" size=+0>
            Category: <i>""" + catStr + """</i><br>
            Cuisine: <i>""" + cuiStr + """</i><br>
            Prep. Time: <i>""" + prepStr + """</i><br>
            Cook Time: <i>""" + cookStr + """</i><br>
            Rating: <i>""" + ratingStr + """</i><br>
            Source: <i>""" + srcStr + """</i><br><br>
            <b>Ingredients:</b> (""" + servStr + """ servings)<br>
            """ + ingStr + """<br><i>* - denotes optional ingredient</i><br><br>
            <b>Instructions:</b><br>""" + instStr + """<br><br>
            <b>Extra Notes:</b><br>""" + notesStr + """</font>"""
            
        return text