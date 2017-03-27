TEMPLATE = subdirs
SUBDIRS = GuetzliLib GuetzliImageIOPlugin GuetzliSimpleGUI

GuetzliLib.subdir = GuetzliLib

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = GuetzliLib
		  
GuetzliSimpleGUI.subdir = GuetzliSimpleGUI
GuetzliSimpleGUI.depends = GuetzliLib GuetzliImageIOPlugin
