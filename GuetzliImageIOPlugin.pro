TEMPLATE = subdirs
SUBDIRS = Guetzli GuetzliImageIOPlugin

Guetzli.subdir = Guetzli

GuetzliImageIOPlugin.subdir = GuetzliImageIOPlugin
GuetzliImageIOPlugin.depends = Guetzli
		  
