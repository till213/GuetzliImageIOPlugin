TEMPLATE = subdirs
SUBDIRS = GuetzliSimpleGUI GuetzliLib GuetzliImageIOPlugin GuetzliImageIOPluginTest  #Guetzli

# Build the GUI first, in order to place the plugin into the application package (macOS)
# later on
GuetzliSimpleGUI.subdir = GuetzliSimpleGUI

GuetzliLib.subdir = GuetzliLib
# Not a real code dependency: we just want to make sure that the GUI is built first,
# in order to place the plugin binary into the application package (macOS)
GuetzliImageIOPlugin.depends = GuetzliSimpleGUI

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = GuetzliLib

GuetzliImageIOPluginTest.subdir = test/GuetzliImageIOPluginTest
GuetzliImageIOPluginTest.depends = GuetzliImageIOPlugin

# Define PNG dependency first
#Guetzli.subdir = Guetzli
#Guetzli.depends = GuetzliLib

OTHER_FILES += README.md \
               LICENSE
