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
import wx.lib.mixins.listctrl as listmix
import images

class ListCtrl(wx.ListView, listmix.ListCtrlAutoWidthMixin, listmix.ColumnSorterMixin):
    itemDataMap = {}
    
    def __init__(self, parent, id, numCols, pos = wx.DefaultPosition, size = wx.DefaultSize,
        style = 0):
            wx.ListView.__init__(self, parent, id, pos, size, style)
            listmix.ListCtrlAutoWidthMixin.__init__(self)
            
            # Arrows on the column headers to show which direction we're sorting
            self.__imageList = wx.ImageList(16, 16)
            self.__sm_up = self.__imageList.Add(images.getSmallUpArrowBitmap())
            self.__sm_dn = self.__imageList.Add(images.getSmallDnArrowBitmap())
            
            self.SetImageList(self.__imageList, wx.IMAGE_LIST_SMALL)    
            listmix.ColumnSorterMixin.__init__(self, numCols)
            
    # Needed by the sorter mixin   
    def GetListCtrl(self):
        return self
    
    def GetSortImages(self):
        return (self.__sm_dn, self.__sm_up)