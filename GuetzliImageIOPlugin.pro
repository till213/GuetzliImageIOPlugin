TEMPLATE = subdirs
SUBDIRS = SimpleGuetzliGUI GuetzliLib GuetzliImageIOPlugin GuetzliImageIOPluginTest

# Build the GUI first, in order to place the plugin into the application package (macOS)
# later on
SimpleGuetzliGUI.subdir = SimpleGuetzliGUI

GuetzliLib.subdir = GuetzliLib
# Not a real code dependency: we just want to make sure that the GUI is built first,
# in order to place the plugin binary into the application package (macOS)
GuetzliImageIOPlugin.depends = SimpleGuetzliGUI

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = GuetzliLib

GuetzliImageIOPluginTest.subdir = test/GuetzliImageIOPluginTest
GuetzliImageIOPluginTest.depends = GuetzliImageIOPlugin

OTHER_FILES += README.md \
               LICENSE
