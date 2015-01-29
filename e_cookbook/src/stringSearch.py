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

# Given a substring, compute the DFA for it's failure shifts.
# Returns: an array of these shift values
def KMPComputeShifts(subStr):
    if subStr == "":
        return []
    
    failShift = [None]*len(subStr)
    failShift[0] = -1;
   
    for i in range(1, len(subStr)):
        temp = failShift[i - 1]
        while temp > -1 and subStr[temp] != subStr[i - 1]:
            temp = failShift[temp]
        failShift[i] = temp + 1
    
    return failShift

# Given a subStr and text, determine if the substring is found in the text.
# Returns: True if found, False if not.
def KMPStringSearch(subStr, text):
    failShift = KMPComputeShifts(subStr)
    return KMPStringSearchWithShifts(subStr, text, failShift)
    
# Given a subStr and text and a failShift array, determine if the substring is
# found in the text. This function is useful if a substring needs to be compared to
# many different pieces of text. The failShift array only needs to be computed once.
# Returns: True if found, False if not.
def KMPStringSearchWithShifts(subStr, text, failShift):    
    # Nothing is in everything!
    if failShift == []:
        return True
    
    subLoc = 0
    txtLoc = 0
    
    while txtLoc < len(text) and subLoc < len(subStr):
        if subLoc == -1 or text[txtLoc] == subStr[subLoc]:
            txtLoc += 1
            subLoc += 1
        # No match, so follow fail link.
        else:
            subLoc = failShift[subLoc]
    
    # Match found.
    if subLoc >= len(subStr):
        return True    # Or return: txtLoc - subLen
    else:
        return False   # Or return: 0