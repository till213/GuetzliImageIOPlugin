#include <QApplication>
#include <QOperatingSystemVersion>
#include <QString>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    if (QOperatingSystemVersion::currentType() == QOperatingSystemVersion::MacOS) {
        // On macOS the plugin directory is inside the application bundle, on the same
        // directory level as the application directory
        QString applicationPluginDirectory = QCoreApplication::applicationDirPath() + "/../plugins";
        QCoreApplication::addLibraryPath(applicationPluginDirectory);
    }    

    MainWindow w;
    w.show();

    return app.exec();
}
