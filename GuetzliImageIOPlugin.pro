TEMPLATE = subdirs
SUBDIRS = Guetzli GuetzliImageIOPlugin GuetzliSimpleGUI

Guetzli.subdir = Guetzli

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = Guetzli
		  
GuetzliSimpleGUI.subdir = GuetzliSimpleGUI
GuetzliSimpleGUI.depends = Guetzli GuetzliImageIOPlugin
