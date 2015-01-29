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
import wx.lib.hyperlink as hl

import mainWindow

ID_BUTTON_CLOSE = wx.NewId()

class AboutDialog(wx.Dialog):
    def __init__(self, parent, id, title):
        wx.Dialog.__init__(self, parent, wx.ID_ANY, title, pos = wx.DefaultPosition,
                         size = (300, 300), style = wx.DEFAULT_DIALOG_STYLE)
        
        self.__buildLayout()
        
    def __buildLayout(self):
        titleFont = wx.Font(9, wx.SWISS, wx.NORMAL, wx.BOLD, False, 'Verdana')
        textFont = wx.Font(9, wx.SWISS, wx.NORMAL, wx.NORMAL, False, 'Verdana')
        
        topsizer = wx.BoxSizer(wx.VERTICAL)
        
        about = wx.StaticText(self, wx.ID_ANY, mainWindow.gAppTitle + " is an electronic recipe organize. It is written in Python, with wxPython and PySqlite.")
        about.SetFont(textFont)
        topsizer.Add(about, 0, wx.ALIGN_LEFT | wx.ALL, 10)
        
        license = wx.StaticText(self, wx.ID_ANY, """You may modify/redistribute the program under the terms of the GPL (version 2 or later).\nA copy of the GPL can be found in the 'COPYING' file distributed with this program. No \nwarranty is provided with this program.""")
        license.SetFont(textFont)
        topsizer.Add(license, 0, wx.ALIGN_LEFT | wx.ALL, 10)
        
        donate = wx.StaticText(self, wx.ID_ANY, """If you find this software useful, please feel free to donate. Donations are accepted through\nthe Sourceforge project page (see link below).""")
        donate.SetFont(textFont)
        topsizer.Add(donate, 0, wx.ALIGN_LEFT | wx.ALL, 10)
        
        proj = wx.StaticText(self, wx.ID_ANY, "Project Homepage:")
        proj.SetFont(titleFont)
        topsizer.Add(proj, 0, wx.ALIGN_CENTER | wx.SOUTH, 3)
        
        projURL = hl.HyperLinkCtrl(self, wx.ID_ANY, "http://sourceforge.net/projects/ecookbook", URL="http://sourceforge.net/projects/ecookbook")
        projURL.SetFont(textFont)
        topsizer.Add(projURL, 0, wx.ALIGN_CENTER | wx.SOUTH, 10)
        
        leadDev = wx.StaticText(self, wx.ID_ANY, "Lead Developer:")
        leadDev.SetFont(titleFont)
        topsizer.Add(leadDev, 0, wx.ALIGN_CENTER | wx.SOUTH, 3)
        
        shyam = wx.StaticText(self, wx.ID_ANY, "Shyam \"Doggan\" Guthikonda")
        shyam.SetFont(textFont)
        shyamEmail = hl.HyperLinkCtrl(self, wx.ID_ANY, "shyamguth@gmail.com", URL="mailto:shyamguth@gmail.com")
        shyamEmail.SetFont(textFont)
        shyamURL = hl.HyperLinkCtrl(self, wx.ID_ANY, "http://ShyamMichael.com", URL="http://shyammichael.com")
        shyamURL.SetFont(textFont)
        shyamSizer = wx.BoxSizer(wx.VERTICAL)
        shyamSizer.Add(shyam, 0, wx.ALIGN_CENTER | wx.SOUTH, 3)
        shyamSizer.Add(shyamEmail, 0, wx.ALIGN_CENTER | wx.SOUTH, 3)
        shyamSizer.Add(shyamURL, 0, wx.ALIGN_CENTER)
        topsizer.Add(shyamSizer, 0, wx.ALIGN_CENTER)
        
        buttonClose = wx.Button(self, ID_BUTTON_CLOSE, "Close")
        wx.EVT_BUTTON(self, ID_BUTTON_CLOSE, self.__onClickClose)
        topsizer.Add(buttonClose, 0, wx.ALIGN_CENTER | wx.ALL, 20)
        
        self.SetSizerAndFit(topsizer)
        
    def __onClickClose(self, e):
        self.Destroy()