TEMPLATE = subdirs
SUBDIRS = GuetzliLib GuetzliImageIOPlugin GuetzliSimpleGUI # Guetzli

GuetzliLib.subdir = GuetzliLib

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = GuetzliLib
		  
GuetzliSimpleGUI.subdir = GuetzliSimpleGUI
GuetzliSimpleGUI.depends = GuetzliLib GuetzliImageIOPlugin

# Define PNG dependency first
#Guetzli.subdir = Guetzli
#Guetzli.depends = GuetzliLib
