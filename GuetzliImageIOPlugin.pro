TEMPLATE = subdirs
SUBDIRS = GuetzliLib GuetzliImageIOPlugin GuetzliImageIOPluginTest GuetzliSimpleGUI #Guetzli

GuetzliLib.subdir = GuetzliLib

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = GuetzliLib

GuetzliImageIOPluginTest.subdir = test/GuetzliImageIOPluginTest
GuetzliImageIOPluginTest.depends = GuetzliImageIOPlugin
		  
GuetzliSimpleGUI.subdir = GuetzliSimpleGUI
GuetzliSimpleGUI.depends = GuetzliLib GuetzliImageIOPlugin

# Define PNG dependency first
#Guetzli.subdir = Guetzli
#Guetzli.depends = GuetzliLib

OTHER_FILES += README.md \
               LICENSE
