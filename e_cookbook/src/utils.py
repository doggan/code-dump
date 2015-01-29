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
import string

# Turns "None" entries into "" so they can be added to the database.
def eliminateNones(aTuple):
    result = []
    for i in range(0, len(aTuple)):
        if aTuple[i] == None:
            result.append(u"")
        else:
            result.append(aTuple[i])
    return result

# Converts a blank string to a -
def blankToDash(aStr):
    if aStr == "":
        aStr = "-"
    return aStr

# Takes a single integer (string) and returns a nicely formated rating string.
# Rating is out of 5.
def ratingFormated(numStr, useDash):
    if numStr == "" and useDash:
        return "-"
    elif numStr == "":
        return ""
    return numStr + "/5"

# Get the ingredient table name in the database, given the unique ID.
def getIngTableName(uniqueID):
    return "ingTable_" + str(uniqueID)

# Converts the bool of the optional checklist to a string representation.
def convertOptionalBool(aBool):
    if aBool:    return "Not Required"
    else    :    return "Required"
    
# Deletes a file. Catches the exception if the file doesn't exist.
def deleteFile(fullPath):
    try:
        os.remove(fullPath)
    except os.error:
        pass
    
# Returns True/False depending on if a file/folder exists.
def isValidLocation(fullPath):
    return os.access(fullPath, os.W_OK)

# The Validator class is used to validate the characters entered into
# a wxTextCtrl. The following flags are the supported types of validation:
DIGIT_ONLY = 1

class Validator(wx.PyValidator):
    def __init__(self, flag = None, pyVar = None):
        wx.PyValidator.__init__(self)
        self.flag = flag
        self.Bind(wx.EVT_CHAR, self.__onEvtChar)
        
    def Clone(self):
        return Validator(self.flag)
    
    def Validate(self, win):
        tc = self.GetWindow()
        val = tc.GetValue()
        
        if self.flag == DIGIT_ONLY:
            for x in val:
                if x not in string.digits:
                    return False
        
        return True
            
    def __onEvtChar(self, e):
        key = e.KeyCode()
        
        if key < wx.WXK_SPACE or key == wx.WXK_DELETE or key > 255:
            e.Skip()
            return
    
        if self.flag == DIGIT_ONLY and chr(key) in string.digits:
            e.Skip()
            return
        
        if not wx.Validator_IsSilent():
            wx.Bell()
            
    # Both of these are needed to fix validator/dialog issue.
    #    See: http://lists.wxwidgets.org/archive/wxPython-users/msg00169.html
    def TransferToWindow(self):
        return True
    
    def TransferFromWindow(self):
        return True