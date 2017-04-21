#include <QApplication>
#include <QSysInfo>
#include <QString>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (QSysInfo::macVersion() != QSysInfo::MV_None) {
        QString applicationPluginDirectory = QCoreApplication::applicationDirPath() + "/../plugins";
        QCoreApplication::addLibraryPath(applicationPluginDirectory);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
