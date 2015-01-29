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

from fraction import *
import utils

class IngredientDialog(wx.Dialog):    
    def __init__(self, parent, id, title):
        wx.Dialog.__init__(self, parent, wx.ID_ANY, title, pos = wx.DefaultPosition,
                         size = wx.DefaultSize, style = wx.DEFAULT_DIALOG_STYLE)
        
        numFields = 7
        self.__fields = []
        for i in range(0, numFields):
            self.__fields.append("")
        
        self.__buildLayout()

    def __buildLayout(self):
        topsizer = wx.BoxSizer(wx.VERTICAL)
        
        sizer1 = wx.BoxSizer(wx.VERTICAL)
        txtName = wx.StaticText(self, wx.ID_ANY, "*Name:")
        self.__txtName = wx.TextCtrl(self, wx.ID_ANY, "", size = (200, -1))
        sizer1.Add(txtName, 0)
        sizer1.Add(self.__txtName, 0, wx.SOUTH)
        
        sizer2 = wx.BoxSizer(wx.HORIZONTAL)
        
        sizerAmt = wx.BoxSizer(wx.VERTICAL)
        txtAmt = wx.StaticText(self, wx.ID_ANY, "Amt: eg. 3 5/8")
        sizerAmtVals = wx.BoxSizer(wx.HORIZONTAL)
        self.__txtAmtWhole = wx.TextCtrl(self, wx.ID_ANY, "", size = (32, -1), validator = utils.Validator(utils.DIGIT_ONLY))
        self.__txtAmtNum = wx.TextCtrl(self, wx.ID_ANY, "", size = (32, -1), validator = utils.Validator(utils.DIGIT_ONLY))
        txtSlash = wx.StaticText(self, wx.ID_ANY, "/")
        self.__txtAmtDen = wx.TextCtrl(self, wx.ID_ANY, "", size = (32, -1), validator = utils.Validator(utils.DIGIT_ONLY))
        sizerAmtVals.Add(self.__txtAmtWhole, 0, wx.EAST, 5)
        sizerAmtVals.Add(self.__txtAmtNum, 0, wx.WEST, 5)
        sizerAmtVals.Add(txtSlash, 0, wx.EAST | wx.WEST, 5)
        sizerAmtVals.Add(self.__txtAmtDen, 0)
        sizerAmt.Add(txtAmt, 0)
        sizerAmt.Add(sizerAmtVals, 0, wx.ALIGN_CENTER)
        
        # TODO: allow user to add new units?????
        sizerUnit = wx.BoxSizer(wx.VERTICAL)
        choiceList = ['-', 'ml', 'l', 'drops', 'dashes', 'pinches', 'tsp', 'tbsp', 'floz', 'cup', 'pt', 'qt', 'gal', 'lb', 'scoop', 'handful']
        txtUnits = wx.StaticText(self, wx.ID_ANY, "*Units:")
        self.__choiceUnits = wx.Choice(self, wx.ID_ANY, (100, 50), choices = choiceList)
        sizerUnit.Add(txtUnits, 0)
        sizerUnit.Add(self.__choiceUnits, 0, wx.ALIGN_CENTER)
        
        sizerOptional = wx.BoxSizer(wx.VERTICAL)
        txtOptional = wx.StaticText(self, wx.ID_ANY, "Optional:")
        self.__checkOptional = wx.CheckBox(self, wx.ID_ANY, "")
        sizerOptional.Add(txtOptional, 0)
        sizerOptional.Add(self.__checkOptional, 0, wx.ALIGN_CENTER)
        
        sizer2.Add(sizerAmt, 0, wx.EAST | wx.WEST, 15)
        sizer2.Add(sizerUnit, 0, wx.EAST, 15)
        sizer2.Add(sizerOptional, 0, wx.EAST, 15)
        
        sizer3 = wx.BoxSizer(wx.VERTICAL)
        txtNotes = wx.StaticText(self, wx.ID_ANY, "Extra Notes:")
        self.__txtNotes = wx.TextCtrl(self, wx.ID_ANY, "", size = (-1, -1))
        sizer3.Add(txtNotes, 0)
        sizer3.Add(self.__txtNotes, 1, wx.ALIGN_CENTER | wx.EXPAND)
        
        # Buttons
        buttonSizer = wx.BoxSizer(wx.HORIZONTAL)
        buttonOk = wx.Button(self, wx.ID_OK)
        wx.EVT_BUTTON(self, buttonOk.GetId(), self.OnOk)
        buttonCancel = wx.Button(self, wx.ID_CANCEL)
        buttonSizer.Add(buttonOk, 0, wx.EAST, 5)
        buttonSizer.Add(buttonCancel, 0, wx.WEST, 5)
        
        topsizer.Add(sizer1, 0, wx.ALIGN_CENTER | wx.NORTH | wx.EAST | wx.WEST, 15)
        topsizer.Add(sizer2, 0, wx.ALIGN_CENTER | wx.ALL, 15)
        topsizer.Add(sizer3, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.EAST | wx.WEST | wx.SOUTH, 15)
        topsizer.Add(buttonSizer, 0, wx.ALIGN_CENTER | wx.EAST | wx.WEST | wx.SOUTH, 20)
        
        self.SetSizerAndFit(topsizer)
        
    def getFields(self):
        self.__fields[0] = (self.__txtName.GetLineText(0))
        self.__fields[1] = (self.__txtAmtWhole.GetLineText(0))
        self.__fields[2] = (self.__txtAmtNum.GetLineText(0))
        self.__fields[3] = (self.__txtAmtDen.GetLineText(0))
        self.__fields[4] = (self.__choiceUnits.GetStringSelection())
        self.__fields[5] = (self.__checkOptional.GetValue())
        self.__fields[6] = (self.__txtNotes.GetLineText(0))
        
        self.__fixAmount()
        
        return self.__fields
    
    def setFields(self, data):
        self.__txtName.SetValue(data[0])
               
        f = toFractionParse(data[1])
        whole = f.getWhole()
        num = f.getNum()
        den = f.getDen()
        if whole == 0 and num == 0:
            self.__txtAmtWhole.SetValue("0")
            self.__txtAmtNum.SetValue("")
            self.__txtAmtDen.SetValue("")
        elif num == 0:
            self.__txtAmtWhole.SetValue(str(whole))
            self.__txtAmtNum.SetValue("")
            self.__txtAmtDen.SetValue("")
        else:
            self.__txtAmtWhole.SetValue(str(whole))
            self.__txtAmtNum.SetValue(str(num))
            self.__txtAmtDen.SetValue(str(den))
        
        self.__choiceUnits.SetStringSelection(data[2])
        
        if data[3] == "Not Required": self.__checkOptional.SetValue(True)
        else:                         self.__checkOptional.SetValue(False)
        
        self.__txtNotes.SetValue(data[4])
    
    def __fixAmount(self):
        if self.__txtAmtWhole.GetLineText(0) == "":
            self.__fields[1] = 0
        if self.__txtAmtNum.GetLineText(0) == "":
            self.__fields[2] = 0
        if self.__txtAmtDen.GetLineText(0) == "":
            self.__fields[3] = 1
    
    def OnOk(self, e):
        # Required fields.
        if self.__txtName.GetLineText(0) == "" or self.__choiceUnits.GetSelection() == -1:
            dlg = wx.MessageDialog(self, "One or more required fields (*) left empty.", "Req. Fields", wx.OK | wx.ICON_ERROR)
            dlg.ShowModal()
            dlg.Destroy()
            return
        
        # Valid amount numbers.
        #  Denominator can't be 0
        #  If Numerator has a value, Denominator can't be blank
        if self.__txtAmtDen.GetLineText(0) == str(0) or (self.__txtAmtNum.GetLineText(0) != "" and self.__txtAmtDen.GetLineText(0) == ""):
            dlg = wx.MessageDialog(self, "Invalid amount value.", "Invalid Amount", wx.OK | wx.ICON_ERROR)
            dlg.ShowModal()
            dlg.Destroy()
            return
        
        e.Skip()