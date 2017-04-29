<p align="center"><img src="https://cloud.githubusercontent.com/assets/203457/24553916/1f3f88b6-162c-11e7-990a-731b2560f15c.png" alt="Guetzli" width="64"></p>

# GuetzliImageIOPlugin

A Qt image plugin (QImageIOPlugin) for the [Google guetzli JPEG encoder](https://github.com/google/guetzli).

# Introduction

Guetzli-generated images are typically 20-30% smaller than images of equivalent quality generated by libjpeg.
Read more about it on the [Google Research Blog](https://research.googleblog.com/2017/03/announcing-guetzli-new-open-source-jpeg.html?m=1).

This plugin incorporates the Google guetzli code into a [QImageIOPlugin](http://doc.qt.io/qt-5/qimageioplugin.html)
for easy use in [Qt](https://www.qt.io/) based applications.

A simple GUI (__SimpleGuetzliGUI__) for testing the plugin is provided, too.

![Simple GuetzliGUI](https://github.com/till213/GuetzliImageIOPlugin/raw/master/doc/img/simpleguetzligui.png "Simple GuetzliGUI on FreeBSD")

# Cloning

This repository includes the Google guetzli repository as Git submodule. The easiest way to fetch all submodules
at once is with the __--recursive__ option:

```
git clone --recursive https://github.com/till213/GuetzliImageIOPlugin
```

Alternatively you can also initialise the submodules after cloning with:

```
git clone https://github.com/till213/GuetzliImageIOPlugin
cd GuetzliImageIOPlugin
git submodule init
git submodule update
```

# Building

The plugin is mainly developed on macOS, but in parallel also on Windows 10 and FreeBSD, using
[Qt 5.6.2](https://www.qt.io/download/) or higher and qmake with *.pro project files.

On Windows the stock binary Qt based on MinGW 4.9.2 is used.

## On POSIX systems

```
cd GuetzliImageIOPlugin
qmake -r
make
```

## On MinGW (Windows)

```
cd GuetzliImageIOPlugin
qmake -r
mingw32-make.exe
```

## Qt Creator (all platforms)

Alternatively you can open the top-level __GuetzliImageIOPlugin.pro__ file in Qt Creator
and build the projects as usual, for instance with __CTRL + B__ (__CMD + B__ on macOS).

This will build all projects, including the GuetzliImageIOPlugin and the SimpleGuetzliGUI.

The GuetzliImageIOPlugin will be located at

```
bin/release/imageformats
```

and the SimpleGuetzliGUI will be located at

```
bin/release
```

## Debug Build

```
qmake -r CONFIG+=debug
make
```

# Installation to Qt Plugin Directory

If you want to install the plugin into the Qt image format plugin directory you can install it as follows:

```
make install
```

This will copy the plugin into the corresponding __imageformats__ folder of the Qt installation.

If Qt is installed system-wide (which is typically the case for Linux/FreeBSD/Unix systems) then
you need __administrator__ rights, for instance:

```
su root
sudo make install
```

## Qt Plugin Directory

With the stock binary Qt distribution, e.g. Qt version 5.6.2 the plugin will be copied into:

Platform    | Image Plugin Location |
----------- | :-------------------- |
macOS       | ~/Qt5.6.2/5.6/clang_64/plugins/imageformats/libGuetzliImageIOPlugin.dylib
Windows     | C:\Qt\Qt5.6.2\5.6\mingw49_32\plugins\imageformats\GuetzliIOPlugin.dll

# Usage

```C++
#include <QApplication>
#include <QImage>
#include <QImageWriter>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QImage image;
  QImageWriter imageWriter;

  // Load or generate some QImage
  image = ...;

  // Set the output file path
  imageWriter.setFileName("guetzli.jpg");

  // The plugin format identifier is simply "guetzli"
  imageWriter.setFormat("GUETZLI");

  // The default quality is set to 85, the lowest possible value is 84
  imageWriter.setQuality(90);

  bool success = imageWriter.write(image);
  if (success) {
    return 0;
  } else {
    // Error processing
    ...
    return -1;
  }
}
```

Alternatively you can also use the [QImage::save](http://doc.qt.io/qt-5/qimage.html#save) method:

```C++
QImage image;

...

bool success = image.save("guetzli.jpg", "GUETZLI", 90);
```

Deploy the GuetzliImageIOPlugin together with your application in a subdirectory called __imageformats__.

```
[application directory]/imageformats/libGuetzliImageIOPlugin.dylib
```

# Credits

This is a simple derivative work done in my free time. All credits go to Robert Obryk and Jyrki Alakuijala, Software Engineers, Google Research Europe.

# License

The Qt plugin code is under the [LGPL 3](LICENSE).

Note that the Google guetzli code comes with its own [Apache License 2.0](https://github.com/google/guetzli/blob/master/LICENSE).



